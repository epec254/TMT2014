var EventSource = require('eventsource');
 
var eventSourceInitDict = {
    headers: {Authorization: "Bearer c9ee389bd9ae4a38ac022c0269eb386d0696f194"}
};
 
var url = "https://api.spark.io/v1/events/";
 
var es = new EventSource(url, eventSourceInitDict);

var fs = require('fs');
var wstream = fs.createWriteStream('myOutput.txt');
//wstream.write('Hello world!\n');
//wstream.write('Another line\n');
//wstream.end();

 
//add your listener
es.addEventListener('bcg-status', function(e) {
    var rawData = JSON.parse(e.data);
    var parsedData = JSON.parse(rawData.data);
    wstream.write(rawData.coreid + ',' + rawData.published_at + ',' + parsedData.id + "," + parsedData.team + "," + parsedData.bcg_status + '\n');
    
    console.log(rawData.coreid + ',' + rawData.published_at + ',' + parsedData.id + "," + parsedData.team + "," + parsedData.bcg_status + '\n'); //result [0,12.14,12.15,548.54,15,457]
});