var express = require('express');
var app = express();
var port = 8000;

var moment = require('moment');
var path = require('path');

var mongojs = require('mongojs');
var lora_db = mongojs('lora_db');

app.use(express.static(path.join(__dirname, 'html')));

app.get('/', function(req, res){
    res.status(200).jsonp({status:"webserver ready"});
});

app.get('/demolora', function(req, res){
    res.sendFile(path.join(__dirname + '/html/demolora.html'));
});

app.get('/getlastdata/:node_id', function(req, res){
    var lora_col = lora_db.collection(String(req.params.node_id));
        lora_col.find({}).limit(1).sort({ts:-1}, function(err, lastdata){
            if(err) return res.status(200).jsonp({status:"error"});
            delete lastdata[0]._id;
            res.status(200).jsonp({status:"ok",data:lastdata[0]});
        });
});

app.listen(port, function(){
    console.log('LoRa IoT platform web server running at port ' + port);
});
