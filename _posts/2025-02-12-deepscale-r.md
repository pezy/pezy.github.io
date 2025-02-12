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
