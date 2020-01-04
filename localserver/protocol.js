var express = require('express');
var app = express();
var port = 31416;

var mongojs = require('mongojs');
var lora_db = mongojs('lora_db');

var moment = require('moment');

app.get('/', function(req, res){
    res.status(200).jsonp({status:"OK"});
});

app.get('/setdata', function(req, res){
    console.log(req.query);
    var lora_col = lora_db.collection(String(req.query.node_id));
        lora_col.insert({
            node_id: String(req.query.node_id),
            temp: Number(req.query.temp),
            humid: Number(req.query.humid),
            rssi: Number(req.query.rssi),
            cnt: Number(req.query.cnt),
            ts: new Date().getTime(),
            ts_pretty: moment(new Date()).format('Do MMMM YYYY, HH:mm:ss')
        }, function(err){
            if(err) return res.status(200).jsonp({status:"error"});
            res.status(200).jsonp({status:"OK"});
        });
});

app.listen(port, function(){
    console.log('Start listen for RPI LoRa gateway at ' + port);
});
