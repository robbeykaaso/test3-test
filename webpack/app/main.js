const greeter = require('./Greeter.js');


window.onBtnSendMsg = async function()
{
    //pipelines().run("unitTest")
    const ret = await greeter()
    document.querySelector("#root").appendChild(ret);
    //var cmd = document.getElementById("待发送消息").value;
    //sendMessage(cmd);
}