const express = require('express');
const fs = require('fs');
var util = require('util');
var SerialPort = require("serialport"); 
const Readline = require('@serialport/parser-readline');
const app = express();
const port = 3000;

var myPort = new SerialPort("COM4", { baudRate: 115200});
let parser = new Readline();
myPort.pipe(parser);
parser.on('data', line => console.log(`${line}`))

var log_file = fs.createWriteStream(__dirname + '/debug.log', {flags : 'w'});
var log_stdout = process.stdout;

console.log = function(d) { //
  log_file.write(util.format(d) + '\n');
  log_stdout.write(util.format(d) + '\n');
};

app.get('/', (req, res) => {
  res.send('Hello World!')
});

app.get("/graphs",function(req, res) {
  res.sendFile("./index.html",  { root: __dirname });
});

app.get("/stocks",function(req, res) {
	//Get stocks.txt
  res.sendFile("./stocks.txt",  { root: __dirname });
});

app.get("/sensors",function(req, res) {
	//Get debug.txt
  res.sendFile("./debug.log",  { root: __dirname });
});

app.listen(port, () => {
  //console.log(`Example app listening at http://localhost:${port}`)
});
