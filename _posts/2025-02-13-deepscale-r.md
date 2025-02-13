---
layout: post
title: "DeepScaleR 本地部署尝试"
tags: [DeepScaleR, local-deployment]
---

> <https://github.com/agentica-project/deepscaler>

据说以 1.5B 的模型规模，超过了 OpenAI 的 o1-preview？

<!--more-->

尝试本地部署了一下，但效果大失所望，感觉可能只有特定场景表现优异吧。

我的考试结果如下:

![image](https://github.com/user-attachments/assets/f9bbf1fa-ce6b-4b30-8dc7-60752838b84d)

不过我之前没有直接部署过 transformer 的预训练模型，这次尝试了一下。

# 环境依赖

1. 先上 [`miniconda`](https://docs.anaconda.com/miniconda/install/)，隔绝环境。
2. 安装 `pytorch` 与 `transformers` 等依赖。

我在 macos M1 Pro 安装命令如下:

```sh
source ~/miniconda3/bin/activate
pip install torch torchvision torchaudio
pip install transformers
```

# 下载并以简单的问答形式运行模型

```py
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

# 检查 MPS 是否可用
device = torch.device(
    "mps") if torch.backends.mps.is_available() else torch.device("cpu")
print(f"Using device: {device}")

# 初始化模型和分词器
model_name = "agentica-org/DeepScaleR-1.5B-Preview"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(
    model_name,
    torch_dtype=torch.float16,  # M1 对 float16 支持很好
    low_cpu_mem_usage=True,     # 降低 CPU 内存使用
)
model = model.to(device)        # 移至 MPS 设备


def generate_response(prompt, max_length=2048):
    # 构建输入
    inputs = tokenizer(prompt, return_tensors="pt").to(device)

    # 存储生成的token ids
    generated_ids = inputs.input_ids  # shape: [1, seq_len]

    with torch.inference_mode():
        for _ in range(max_length):
            # 获取模型输出
            outputs = model(generated_ids)
            # shape: [1, vocab_size]
            next_token_logits = outputs.logits[:, -1, :]
            next_token_logits = next_token_logits / 0.7

            # 应用 top-p 采样
            probs = torch.softmax(next_token_logits, dim=-1)
            sorted_probs, sorted_indices = torch.sort(probs, descending=True)
            cumsum_probs = torch.cumsum(sorted_probs, dim=-1)
            idx_to_remove = cumsum_probs > 0.9
            idx_to_remove[..., 1:] = idx_to_remove[..., :-1].clone()
            idx_to_remove[..., 0] = 0
            sorted_probs[idx_to_remove] = 0.0
            sorted_probs = sorted_probs / \
                sorted_probs.sum(dim=-1, keepdim=True)

            # 采样下一个token
            next_token_id = torch.multinomial(
                sorted_probs[0], num_samples=1)  # shape: [1]
            next_token = sorted_indices[0, next_token_id]  # shape: [1]

            # 将新token添加到序列中 (确保维度正确)
            next_token = next_token.view(1, 1)  # shape: [1, 1]
            generated_ids = torch.cat(
                [generated_ids, next_token], dim=1)  # 在序列长度维度上拼接

            # 解码并打印新token
            new_text = tokenizer.decode(
                next_token[0], skip_special_tokens=True)
            print(new_text, end='', flush=True)

            # 检查是否生成了结束标记
            if next_token.item() == tokenizer.eos_token_id:
                break

    print()
    return tokenizer.decode(generated_ids[0], skip_special_tokens=True)


def clear_memory():
    if torch.backends.mps.is_available():
        torch.mps.empty_cache()


while True:
    try:
        user_input = input("\n用户: ")
        if user_input.lower() in ['退出', 'quit', 'exit']:
            break

        print("\n助手: " + generate_response(user_input))
        clear_memory()

    except KeyboardInterrupt:
        print("\n程序已终止")
        break
    except Exception as e:
        print(f"\n发生错误: {e}")
        continue
```

而且输出的速度巨慢，可能与量化精度有关？

---

我初步怀疑，直接使用 Transformer 这种通用、功能丰富的加载器，可能会有很多动态检查、自动调度以及不太针对推理优化的工作，这将是导致启动和解析龟速的一个原因。

我还是应该尝试主流的 GGUF 格式。

先写一个简单的转换脚本，转为 `pytorch` 的 `safetensors` 格式:

```python
from transformers import AutoModelForCausalLM, AutoTokenizer


# 加载模型和分词器
model_name = "agentica-org/DeepScaleR-1.5B-Preview"
model = AutoModelForCausalLM.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)

# 将模型保存为 PyTorch 格式
output_dir = "deepscaler_model"
model.save_pretrained(output_dir, safe_serialization=True)  # 使用 safetensors 格式
tokenizer.save_pretrained(output_dir)
```

然后就可以利用 `llama.cpp` 的 `convert_hf_to_gguf.py` 转换为 GGUF 格式:

```sh
python llama.cpp/convert_hf_to_gguf.py deepscaler_model --outfile deepscaler_model.gguf
```

拿到 `GGUF` 模型后，我以为就可以利用 `ollama` 轻松运行了:

```sh
ollama start
ollama create deepscaler_model.gguf
```

结果给我来了个:

```sh
parsing GGUF
Error: supplied file was not in GGUF format
```

咨询了下 ChatGPT，可能是 DeepScaleR 是基于 `qwen2` 而来，而 `ollama` 并无法直接支持 `qwen2` 的模型。

突然想到 `Jan` 对 `qwen2` 的模型支持还可以，于是用 `Jan` 加载了一下这个 `gguf`，果然直接可用了。

解析输出速度巨快。

不过我发现，中文问答会出一些岔子，回答完问题后会自行解析一个算术题。很是莫名其妙。

但对于英文问题，表现基本是比较稳定的。不过要说跟 o1-preview 比，我个人觉得在推理方面，还是有不小差距的。就不知道专业的 PK 具体是如何进行的了。
