//#region control
let test_sum = 0
let test_pass = 0

// 向qt发送消息
function sendMessage(msg)
{
    if(typeof context == 'undefined')
    {
        alert("context对象获取失败！");
    }
    else
    {
        context.onMsg(msg);
    }
}

function recvMessage(msg)
{
    alert("接收到Qt发送的消息：" + msg);
}

function onBtnSendMsg()
{
//    for (let i = 0; i < 100; ++i)
        pipelines().run("unitTest")
    //var cmd = document.getElementById("待发送消息").value;
}

function onBtnReportCLeak()
{
    pipelines().run("reportCLeak", 0)
}

function onBtnReportQMLLeak()
{
    pipelines().run("reportQMLLeak", 0)
}

function onBtnReportJSLeak()
{
    pipelines().run("reportJSLeak", 0)
}

pipelines().find("testStorage")
.nextF(function(aInput){
    test_sum++
    aInput.setData("Pass: testStorage").out()
})
.next("testSuccessJS")

function onBtnTestStorage(){
    pipelines().run("testStorage", {})
}

pipelines().find("testAWSStorage")
.nextF(function(aInput){
    test_sum++
    aInput.setData("Pass: testAWSStorage").out()
})
.next("testSuccessJS")

function onBtnTestAWSStorage(){
    pipelines().run("testAWSStorage", {})
}

var vis = false
function onBtnModifyQSGBoard(){
    vis = !vis
    pipelines().run("js_updateQSGAttr_testbrd", [{key: ["arrow", "visible"], val: vis}], "wholeArrowVisible")
}

pipelines().find("js_QSGAttrUpdated_testbrd")
.nextF(function(aInput){
    test_sum++
    aInput.setData("Pass: js_updateQSGAttr").out()
})
.next("testSuccessJS")

async function onBtnShowImage()
{
    let dt = await pipelines().input({folder: false, filter: ["Image files (*.jpg *.png *.jpeg *.bmp)"]}, "test24").asyncCall("js_selectFile", "js", true)
    if (!dt.data().length)
        return
    dt = await pipelines().input(false, "test24", new scopeCache({path: dt.data()[0]})).asyncCall("js_readImage", "js", true)
    if (!dt.data())
        return
    let img = new Image(400, 300)
    const cvs = document.getElementById("cvs")
    const ctx = cvs.getContext("2d")
    img.onload = function(){
        ctx.drawImage(img, 0, 0, 400, 300)
    }
    img.src = dt.scope().data("uri")

    await pipelines().input(false, "test24", new scopeCache({path: "test.png", uri: dt.scope().data("uri")})).asyncCall("js_writeImage", "js", true)
}

//#endregion

//#region test

//#region test1;test2;test3;test4

function test1_(){
    //test1 -> test1_next -> testSuccessJS
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 3)
        aInput.out()
    }, {name: "test1"})
    .nextF(function(aInput){  //test1_next
        console.assert(aInput.data() == 3)
        aInput.outs("Pass: test1", "testSuccessJS")
    })
}

function test1(){
    pipelines().run("test1", 3)
}

function test2_(){
    //test2 -> test2_ -> testSuccessJS
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 4)
        aInput.outs(5, "test2_")
    }, {name: "test2"})
    .nextF(function(aInput){
        console.assert(aInput.data() == 5)
        aInput.outs("Pass: test2", "testSuccessJS")
    }, "", {name: "test2_"})
}

function test2(){
    pipelines().run("test2", 4)
}

function test3_(){
    //test3 -> test3_0 -> test3__ -> testSuccessJS
    //                 -> testSuccessJS
    //test3_1 -> test3__
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 66)
        aInput.outs("test3", "test3_0")
    }, {name: "test3"})
    .next("test3_0")
    .next("testSuccessJS")

    pipelines().add(function(aInput){
        aInput.out()
    }, {name: "test3_1"})
    .next("test3__")
    .next("testSuccessJS")

    pipelines().find("test3_0")
    .nextF(function(aInput){
        aInput.out()
    }, "", {name: "test3__"})
    .next("testSuccessJS")

    pipelines().add(function(aInput){
        console.assert(aInput.data() == "test3")
        aInput.outs("Pass: test3", "testSuccessJS")
        aInput.outs("Pass: test3_", "test3__")
    }, {name: "test3_0"})
}

function test3(){
    pipelines().run("test3", 66)
    pipelines().run("test3_1", "Pass: test3__")
}

function test4(){
    //test4 -> test4_(ex) -> test4__(ex) -> test_4 -> testSuccessJS
    pipelines().find("test4_").removeNext("test4__")
    pipelines().find("test4__").removeNext("test_4")

    pipelines().add(function(aInput){
        aInput.scope().cache("hello", "world")
        aInput.out()
    }, {name: "test4"})
    .next("test4_")
    .next("test4__")
    .nextF(function(aInput){  //test4__next
        console.assert(aInput.data() == 6)
        console.assert(aInput.scope().data("hello") == null)
        console.assert(aInput.scope().data("hello2") == "world")
        aInput.outs("Pass: test4", "testSuccessJS")
    }, "", {name: "test_4"})

    pipelines().run("test4", 4)
}

//#endregion

//#region test5;test6;test7;test8

function test5(){
    //test5(ex) -> test_5 -> testSuccessJS
    pipelines().find("test5").removeNext("test_5")

    pipelines().find("test5")
    .nextF(function(aInput){
        console.assert(aInput.data() == "world")
        aInput.outs("Pass: test5", "testSuccessJS")
    }, "", {name: "test_5"})

    pipelines().run("test5", "hello")
}

function test6(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 4)
        console.assert(aInput.scope().data("hello") == "world")
        aInput.setData(aInput.data() + 1).out()
    }, {name: "test6_", external: "c++"})

    pipelines().add(function(aInput){
        console.assert(aInput.data() == 5)
        aInput.scope(true).cache("hello2", "world");
        aInput.setData(aInput.data() + 1).out()
    }, {name: "test6__", external: "c++"})

    return "test6"
}

function test7(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == "hello")
        aInput.setData("world").out()
    }, {name: "test7", external: "c++"})

    return "test7"
}

function test8(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == "hello")
        aInput.outs("Pass: test8", "testSuccessJS")
    }, {name: "test8", external: "c++"})

    return "test8"
}

function test8_(){

}

//#endregion

//#region test9;test11;test12;test13

function test9(){
    pipelines().run("test9", "hello")
    return "test9"
}

function test9_(){
    return "test9_"
}

function test11(){
    return "test11"
}

function test12(){
    return "test12"
}

function test13(){
    return "test13"
}

//#endregion

//#region test14;test15;test16;test17

function test14(){
    return "test14"
}

function test15_(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 66)
        aInput.setData(77).out()
    }, {name: "test15", type: "Partial"})
    .nextFB(function(aInput){
        console.assert(aInput.data() == 77)
        aInput.outs("Pass: test15", "testSuccessJS")
    }, "test15")
    .nextF(function(aInput){
        console.assert(aInput.data() == 77)
        aInput.outs("Fail: test15", "testFailJS")
    }, "test15_")
}

function test15(){
    pipelines().run("test15", 66, "test15")
}

function test16_(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 66)
        aInput.setData(77).out()
    }, {name: "test16", external: "c++", type: "Partial"})
}

function test16(){
    return "test16"
}

function test17_(){
    pipelines().find("test17")
    .nextFB(function(aInput){
        console.assert(aInput.data() == 77.0)
        aInput.outs("Pass: test17", "testSuccessJS")
    }, "test17", {name: "test17_"})
    .nextF(function(aInput){
        console.assert(aInput.data() == 77.0)
        aInput.outs("Fail: test17", "testFailJS")
    }, "test17_", {name: "test17__"})

}

function test17(){
    pipelines().run("test17", 66, "test17")
}

//#endregion

//#region test18;test19;test20

function test18(){
    return "test18"
}

function test19_(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 66.0)
        aInput.out()
    }, {name: "test19_0",
        delegate: "test19",
        type: "Delegate"})
    .next("testSuccessJS")

    pipelines().add(function(aInput){
        console.assert(aInput.data() == 56.0)
        aInput.setData("Pass: test19").out()
    }, {name: "test19"})
}

function test19(){
    pipelines().run("test19_0", 66.0)
    pipelines().run("test19", 56.0)
}

function test20(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 56.0)
        aInput.setData("Pass: test20").out()
    }, {name: "test20", external: "c++"})

    return "test20"
}

function test20_(){

}

//#endregion

//#region test21;test22;test23;test24

function test21(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 66.0)
        aInput.out()
    }, {name: "test21_0",
        delegate: "test21",
        type: "Delegate"})
    .nextB("testSuccess")
    .next("testSuccessJS")

    pipelines().run("test21_0", 66)

}

function test21_(){
    return "test21_"
}

function test21__(){
    pipelines().run("test21", 56)

    return "test21__"
}

function test22(){
    return "test22"
}

async function test23(){
    let stm = await pipelines().input(0, "test23").asyncCallF(function(aInput){
            aInput.setData(aInput.data() + 1).out()
        })
    stm = await stm.asyncCallF(function(aInput){
        console.assert(aInput.data() == 1)
        aInput.outs("world")
    })
    stm = await stm.asyncCallF(function(aInput){
        console.assert(aInput.data() == "world")
        aInput.setData("Pass: test23").out()
    })
    await stm.asyncCall("testSuccessJS")
}

async function test24(){
    let stm = await pipelines().input(24, "test24").asyncCall("test24", "js", true)
    await stm.asyncCall("testSuccessJS")
}

//#endregion

//#region test25;test26;test27;test28

function test25_(){
    pipelines().add(function(aInput){
        console.assert(aInput.data() == 25.0)
        aInput.setData("Pass: test25").out()
    }, {name: "test25", external: "c++"})
}

function test25(){
    return "test25"
}

function test26(){
    return "test26"
}

function test27(){
    return "test27"
}

function test28(){
    return "test28"
}

//#endregion

//#region test29;test30;test31;test32

function test29(){
    pipelines().add(function(aInput){
        context = aInput.scope().data("ctx")
        console.assert(aInput.scope().data("ctx") == context)
        sendMessage("lala")
    }, {name: "test29", external: "c++"})

    return "test29"
}

function test30(){
    return "test30"
}

function test31(){
    return "test31"
}

function test32(){
    return "test32"
}

//#endregion

//#region test33;test34;test35;test36
function test33(){
    return "test33"
}

function test34(){
    return "test34"
}

function test35(){
    return "test35"
}

function test36(){
    return "test36"
}

//#endregion

//#region test37;test38;test39; test40

function test37(){
    return "test37"
}

function test38(){
    return "test38"
}

function test39(){
    return "test39"
}

function test40(){
    return "test40"
}

//#endregion

//#region test41;test42;test43;test44

function test41(){
    return "test41"
}

function test42(){
    return "test42"
}

function test42_(){
    return "test42"
}

function test43(){
    return "test43"
}

function test43_(){
    return "test43_"
}

function test43__(){
    return "test43__"
}

function test44(){
    return "test44"
}

function test45(){
    return "test45"
}

//#endregion

//#region test46;test47;test48;test49

function test46(){
    return "test46"
}

function test47(){
    return "test47"
}

function test48(){
    return "test48"
}

class pipeCustomJS extends pipe{

    execute(aStream){
        aStream.scope().cache("flag", "test49")
        this.doEvent(aStream)
        this.doNextEvent(this.m_next, aStream)
    }
}

pipelines().add(function(aInput){
    const sp = aInput.scope()
    aInput.setData(new pipeCustomJS(sp.data("parent"), sp.data("name")))
}, {name: "createJSPipeCustomJS"})

function test49_(){
    pipelines().add(function(aInput){
        console.assert(aInput.scope().data("flag") == "test49")
        aInput.outs("Pass: test49", "testSuccessJS")
    }, {name: "test49", type: "CustomJS"})
}

function test49(){
    pipelines().run("test49", 0)
}

//#endregion

//#region test50;test51;test52

function test50_(){
    pipelines().add(function(aInput){
        aInput.setData(1).out()
    }, {name: "test50"})
    .next("test50_c")
    .next("test50_qml")
    .nextF(function(aInput){
        console.assert(aInput.data() == 1)
        aInput.outs("Pass: test50", "testSuccessJS")
    })
}

function test50(){
    pipelines().run("test50", 0)
}

function test51(){
    pipelines().add(function(aInput){
        let dt = aInput.data()
        console.assert(dt == 1.0)
        aInput.setData(dt + 1).out()
    }, {name: "test__51", before: "test_51", replace: true})

    pipelines().add(function(aInput){
        let dt = aInput.data()
        console.assert(dt == 2.0)
        aInput.setData(dt + 1).out()
    }, {name: "test_51", before: "test51", replace: true})

    pipelines().add(function(aInput){
        let dt = aInput.data()
        console.assert(dt == 3.0)
        aInput.setData(dt + 1).out()
    }, {name: "test51", replace: true})

    pipelines().add(function(aInput){
        let dt = aInput.data()
        console.assert(dt == 4.0)
        aInput.setData(dt + 1).out()
    }, {name: "test51_", after: "test51", replace: true})

    pipelines().add(function(aInput){
        let dt = aInput.data()
        console.assert(dt == 5.0)
        aInput.outs("Pass: test51", "testSuccessJS")
    }, {name: "test51__", after: "test51_", replace: true})

    pipelines().run("test51", 1)
}

function test52(){
    return "test52"
}
//#endregion

pipelines().add(function(aInput){
    test_pass++
    console.log("Success: " + aInput.data() + "(" + test_pass + "/" + test_sum + ")")
    aInput.out()
}, {name: "testSuccessJS"})

pipelines().add(function(aInput){
    test_pass--
    console.log("Fail: " + aInput.data() + "(" + test_pass + "/" + test_sum + ")")
    aInput.out()
}, {name: "testFailJS"})

test1_()
test2_()
test3_()
test15_()
test16_()
test17_()
test19_()
test25_()
test49_()
test50_()

    pipelines().add(function(aInput){
        //js
        let test = [
            [test1, 1], //test js anonymous next
            [test2, 1], //test js specific next
            [test3, 3], //test js pipe future
            [test4, 1], //test pipe mixture: js->js.future(c++)->js.future(c++)->js ; scopeCache
            [test5, 1], //test pipe mixture: js.future(c++)->js

            [test8_, 1],
            [test15, 1], //test js pipe partial
            [test17, 1], //test pipe mixture partial: js.future(c++)->js
            [test19, 1], //test js pipe delegate and pipe param

            [test20_, 1], //test pipe mixture delegate: c++->c++.future(js)->js, c++

            [test21, 1],  //test pipe mixture delegate: js->js.future(c++)->c++, js
            [test23, 1], //test js asyncCall

            [test24, 1], //test pipe mixture: js.asyncCall.c++
            [test49, 1],  //test custom js pipe
            [test50, 1],  //test js->js.future(c++)->js.future(qml)->js
            [test51, 1] //test js aop and keep topo
        ]
        for (let i in test)
            test_sum += test[i][1]
        for (let i in test)
            test[i][0]()
        //qml
        aInput.outs({
                        [test31()]: 1, //test qml anonymous next
                        [test32()]: 1, //test qml specific next
                        [test33()]: 3,  //test qml pipe future
                        [test34()]: 1,  //test pipe mixture: qml->qml.future(c++)->qml.future(c++)->qml; scopeCache

                        [test35()]: 1, //test pipe mixture: qml.future(c++)->qml,
                        [test38()]: 1, //test qml pipe partial
                        [test39()]: 1, //test pipe mixture partial: qml.future(c++)->qml
                        [test41()]: 1, //test qml pipe delegate and pipe param
                        [test42_()]: 1, //test pipe mixture delegate: c++->c++.future(qml)->qml, c++
                        [test43()]: 1,  //test pipe mixture delegate: qml->qml.future(c++)->c++, qml,
                        [test44()]: 1, //test qml asyncCall

                        [test45()]: 1, //test pipe mixture: qml.asyncCall.c++
                        [test47()]: 1, //test qml aop and keep topo
                        [test48()]: 1, //test custom qml pipe
                    }, "unitTestQML")
        aInput.outs({
            //js
            [test6()]: 1, //test pipe mixture: c++->c++.future(js)->c++.future(js)->c++ ; scopeCache
            [test7()]: 1, //test pipe mixture: c++.future(js)->c++
            [test8()]: 0, //test pipe mixture: c++.future(js)
            [test9_()]: 1, //test pipe mixture: js.future(c++)
            [test16()]: 1, //test pipe mixture partial: c++.future(js)->c++
            [test20()]: 1, //test pipe mixture delegate: c++->c++.future(js)->js, c++
            [test21_()]: 1,  //test pipe mixture delegate: js->js.future(c++)->c++, js
            [test25()]: 1, //test pipe mixture: c++.asyncCall.js
            [test29()]: 1,  //test rea-js arbitrary type

            //c++
            [test11()]: 1, //test c++ anonymous next
            [test12()]: 1, //test c++ specific next
            [test13()]: 3, //test c++ pipe future
            [test14()]: 1, //test c++ pipe partial
            [test18()]: 1, //test c++ pipe delegate and pipe param

            [test22()]: 1, //test c++ asyncCall
            [test26()]: 1, //test c++ aop and keep topo
            [test27()]: 1, //test c++ functor
            [test30()]: 1, //test c++ pipe parallel
            [test52()]: 1, //test c++ pipe parallel2

            //qml
            [test28()]: 1, //test pipe qml
            [test36()]: 1, //test pipe mixture: c++->c++.future(qml)->c++.future(qml)->c++ ; scopeCache
            [test37()]: 1, //test pipe mixture: c++.future(qml)->c++
            [test40()]: 1, //test pipe mixture partial: c++.future(qml)->c++
            [test42()]: 1, //test pipe mixture delegate: c++->c++.future(qml)->qml, c++
            [test43_()]: 1,  //test pipe mixture delegate: qml->qml.future(c++)->c++, qml
            [test46()]: 1, //test pipe mixture: c++.asyncCall.qml
                    }, "unitTestC++")
    }, {name: "unitTest"})
    .next("unitTestC++")
    .nextF(function(aInput){
        aInput.setData({
            //js
            [test9()]: 0,
            [test21__()]: 0,
            //qml
            [test43__()]: 0

        }).out()
    })
    .next("unitTestQML")

//#endregion
