const greeter = require('./Greeter.js');
const jstree_demo = require('./jsTree')

window.onBtnSendMsg = async function()
{
    //pipelines().run("unitTest")
    const ret = await greeter()
    document.querySelector("#root").appendChild(ret);
    jstree_demo()
    //var cmd = document.getElementById("待发送消息").value;
    //sendMessage(cmd);
}