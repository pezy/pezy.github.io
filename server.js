var express = require('express');
var app 	= express();

var bodyParser		= require('body-parser');
var errorHandler 	= require('errorhandler');
var methodOverride  = require('method-override');
var serveStatic     = require('serve-static');

var hostname = process.env.HOSTNAME || 'localhost';
var port = parseInt(process.env.PORT, 10) || 8686;

app.use(methodOverride());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
	extended: true
}));
app.use(serveStatic(__dirname));
app.use(errorHandler({
	dumpExceptions: true,
	showStack: true
}));

app.get("/", function( req, res ) {
	res.redirect('/index.html');
});

console.log("server running %s on http://%s:%s", __dirname, hostname, port);
app.listen(port, hostname);
