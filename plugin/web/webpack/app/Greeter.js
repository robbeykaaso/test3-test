var config = require('./config.json');
var rea = require('./rea.js')

rea.pipelines().add(function(aInput){
  aInput.setData(config.greetText).out()
}, {name: "test"})

module.exports = async function() {
    let greet = document.createElement('div');
    let ret = await rea.pipelines().input().asyncCallS("test")
    greet.textContent = ret.data()
    return greet;
};