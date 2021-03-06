import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.3
import QtWebEngine 1.8
import QtWebChannel 1.0
import "qml/gui/Basic"
import "qml/gui/Pipe"
import "qml/gui/Custom"
import "qml/gui/Pipe/TreeNodeView"
import QSGBoard 2.0

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    property var view_cfg
    property int test_pass
    property int test_sum
    property var unit_test
    Universal.theme: Universal.Dark

    menuBar: MenuBar{
        id: mainmenu
        Menu{
            title: "rea"

            MenuItem{
                text: "gTest"
                onClicked: Pipelines().run("gTest", 0)
            }

            MenuItem{
                text: "breakpad"
                onClicked: Pipelines().run("breakpad", 0)
            }

            MenuItem{
                text: "test"
                onClicked: Pipelines().run("openWebWindow", 0)
            }
            function test28(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data()["test28"] === "test28")
                    aInput.outs(aInput.data(), "test28_0")
                }, {name: "test28_", external: "c++"})
            }

            function test31_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 3)
                    aInput.out()
                }, {name: "test31"})
                .nextF(function(aInput){
                    console.assert(aInput.data() == 3)
                    aInput.outs("Pass: test31", "testSuccessQML")
                })
            }

            function test31(){
                Pipelines().run("test31", 3)
            }

            function test32_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 4)
                    aInput.outs(5, "test32_")
                }, {name: "test32"})
                .nextF(function(aInput){
                    console.assert(aInput.data() == 5)
                    aInput.outs("Pass: test32", "testSuccessQML")
                }, "", {name: "test32_"})
            }

            function test32(){
                Pipelines().run("test32", 4)
            }

            function test33_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 66)
                    aInput.outs("test33", "test33_0")
                }, {name: "test33"})
                .next("test33_0")
                .next("testSuccessQML")

                Pipelines().add(function(aInput){
                    aInput.out()
                }, {name: "test33_1"})
                .next("test33__")
                .next("testSuccessQML")

                Pipelines().find("test33_0")
                .nextF(function(aInput){
                    aInput.out()
                }, "", {name: "test33__"})
                .next("testSuccessQML")

                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == "test33")
                    aInput.outs("Pass: test33", "testSuccessQML")
                    aInput.outs("Pass: test33_", "test33__")
                }, {name: "test33_0"})
            }

            function test33(){
                Pipelines().run("test33", 66)
                Pipelines().run("test33_1", "Pass: test33__")
            }

            function test34(){
                Pipelines().find("test34_").removeNext("test34__")
                Pipelines().find("test34__").removeNext("test_34")

                Pipelines().add(function(aInput){
                    aInput.scope().cache("hello", "world")
                    aInput.out()
                }, {name: "test34"})
                .next("test34_")
                .next("test34__")
                .nextF(function(aInput){  //test34__next
                    console.assert(aInput.data() == 6)
                    console.assert(aInput.scope().data("hello") == null)
                    console.assert(aInput.scope().data("hello2") == "world")
                    aInput.outs("Pass: test34", "testSuccessQML")
                }, "", {name: "test_34"})

                Pipelines().run("test34", 4)
            }

            function test35(){
                Pipelines().find("test35").removeNext("test_35")

                Pipelines().find("test35")
                .nextF(function(aInput){
                    console.assert(aInput.data() == "world")
                    aInput.outs("Pass: test35", "testSuccessQML")
                }, "", {name: "test_35"})

                Pipelines().run("test35", "hello")
            }

            function test36(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 4)
                    console.assert(aInput.scope().data("hello") == "world")
                    aInput.setData(aInput.data() + 1).out()
                }, {name: "test36_", external: "c++"})

                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 5)
                    aInput.scope(true).cache("hello2", "world");
                    aInput.setData(aInput.data() + 1).out()
                }, {name: "test36__", external: "c++"})
            }

            function test37(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == "hello")
                    aInput.setData("world").out()
                }, {name: "test37", external: "c++"})
            }

            function test38_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 66)
                    aInput.setData(77).out()
                }, {name: "test38", type: "Partial"})
                .nextFB(function(aInput){
                    console.assert(aInput.data() == 77)
                    aInput.outs("Pass: test38", "testSuccessQML")
                }, "test38")
                .nextF(function(aInput){
                    console.assert(aInput.data() == 77)
                    aInput.outs("Fail: test38", "testFailQML")
                }, "test38_")
            }

            function test38(){
                Pipelines().run("test38", 66, "test38")
            }

            function test39_(){
                Pipelines().find("test39")
                .nextFB(function(aInput){
                    console.assert(aInput.data() == 77.0)
                    aInput.outs("Pass: test39", "testSuccessQML")
                }, "test39", {name: "test39_"})
                .nextF(function(aInput){
                    console.assert(aInput.data() == 77.0)
                    aInput.outs("Fail: test39", "testFailQML")
                }, "test39_", {name: "test39__"})

            }

            function test39(){
                Pipelines().run("test39", 66, "test39")
            }

            function test40(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 66)
                    aInput.setData(77).out()
                }, {name: "test40", external: "c++", type: "Partial"})
            }

            function test41_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 66.0)
                    aInput.out()
                }, {name: "test41_0",
                    delegate: "test41",
                    type: "Delegate"})
                .next("testSuccessQML")

                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 56.0)
                    aInput.setData("Pass: test41").out()
                }, {name: "test41"})
            }

            function test41(){
                Pipelines().run("test41_0", 66.0)
                Pipelines().run("test41", 56.0)
            }

            function test42(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 56.0)
                    aInput.setData("Pass: test42").out()
                }, {name: "test42", external: "c++"})
            }

            function test43(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 66.0)
                    aInput.out()
                }, {name: "test43_0",
                    delegate: "test43",
                    type: "Delegate"})
                .nextB("testSuccess")
                .next("testSuccessQML")

                Pipelines().run("test43_0", 66)
            }

            function test43__(){
                Pipelines().run("test43", 56)
            }

            function test44(){
                Pipelines().input(0, "test44")
                .asyncCallF(function(aInput){
                    aInput.setData(aInput.data() + 1).out()
                }).asyncCallF(function(aInput){
                    console.assert(aInput.data() == 1)
                    aInput.outs("world")
                }).asyncCallF(function(aInput){
                    console.assert(aInput.data() == "world")
                    aInput.setData("Pass: test44").out()
                }).asyncCall("testSuccessQML")
            }

            function test45(){
                Pipelines().input(24, "test45")
                .asyncCall("test45", true, "qml", true)
                .asyncCall("testSuccessQML", false)
            }

            function test46(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 25.0)
                    aInput.setData("Pass: test46").out()
                }, {name: "test46", external: "c++"})
            }

            function test47(){
                Pipelines().add(function(aInput){
                    let dt = aInput.data()
                    console.assert(dt == 1.0)
                    aInput.setData(dt + 1).out()
                }, {name: "test__47", before: "test_47", replace: true})

                Pipelines().add(function(aInput){
                    let dt = aInput.data()
                    console.assert(dt == 2.0)
                    aInput.setData(dt + 1).out()
                }, {name: "test_47", before: "test47", replace: true})

                Pipelines().add(function(aInput){
                    let dt = aInput.data()
                    console.assert(dt == 3.0)
                    aInput.setData(dt + 1).out()
                }, {name: "test47", replace: true})

                Pipelines().add(function(aInput){
                    let dt = aInput.data()
                    console.assert(dt == 4.0)
                    aInput.setData(dt + 1).out()
                }, {name: "test47_", after: "test47", replace: true})

                Pipelines().add(function(aInput){
                    let dt = aInput.data()
                    console.assert(dt == 5.0)
                    aInput.outs("Pass: test47", "testSuccessQML")
                }, {name: "test47__", after: "test47_", replace: true})

                Pipelines().run("test47", 1)
            }

            function test48_(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.scope().data("flag") == "test48")
                    aInput.outs("Pass: test48", "testSuccessQML")
                }, {name: "test48", type: "CustomQML"})
            }

            function test48(){
                Pipelines().run("test48", 0)
            }

            function test50(){
                Pipelines().add(function(aInput){
                    console.assert(aInput.data() == 1.0)
                    aInput.out()
                }, {name: "test50_qml", external: "js"})
            }

            Component.onCompleted: {
                Pipelines().add(function(aInput){
                    test_pass++
                    console.log("Success: " + aInput.data() + "(" + test_pass + "/" + test_sum + ")")
                    aInput.out()
                }, {name: "testSuccessQML"})

                Pipelines().add(function(aInput){
                    test_pass--
                    console.log("Fail: " + aInput.data() + "(" + test_pass + "/" + test_sum + ")")
                    aInput.out()
                }, {name: "testFailQML"})

                unit_test = {
                    test31: test31,
                    test32: test32,
                    test33: test33,
                    test34: test34,
                    test35: test35,
                    test38: test38,
                    test39: test39,
                    test40: test40,
                    test41: test41,
                    test42: test42,
                    test43: test43,
                    test43__: test43__,
                    test44: test44,
                    test45: test45,
                    test47: test47,
                    test48: test48
                }
                test28()
                test31_()
                test32_()
                test33_()
                test36()
                test37()
                test38_()
                test39_()
                test40()
                test41_()
                test46()
                test48_()
                test50()

                Pipelines().add(function(aInput){
                    var dt = aInput.data()
                    for (var i in dt)
                        test_sum += dt[i]
                    for (var i in dt)
                        if (unit_test[i])
                            unit_test[i]()
                    gc()
                }, {name: "unitTestQML", external: "js"})
            }
        }

        Menu{
            title: "qsgShow"

            delegate: MenuItem {
                id: menuItem
                implicitWidth: 200
                implicitHeight: 40

                arrow: Canvas {
                    x: parent.width - width
                    implicitWidth: 40
                    implicitHeight: 40
                    visible: menuItem.subMenu
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
                        ctx.moveTo(15, 15)
                        ctx.lineTo(width - 15, height / 2)
                        ctx.lineTo(15, height - 15)
                        ctx.closePath()
                        ctx.fill()
                    }
                }

                indicator: Item {
                    implicitWidth: 40
                    implicitHeight: 40
                    Rectangle {
                        width: 14
                        height: 14
                        anchors.centerIn: parent
                        visible: menuItem.checkable
                        border.color: "#21be2b"
                        radius: 3
                        Rectangle {
                            width: 8
                            height: 8
                            anchors.centerIn: parent
                            visible: menuItem.checked
                            color: "#21be2b"
                            radius: 2
                        }
                    }
                }

                contentItem: Text {
                    leftPadding: menuItem.indicator.width
                    rightPadding: menuItem.arrow.width
                    text: menuItem.text
                    font: menuItem.font
                    opacity: enabled ? 1.0 : 0.3
                    color: menuItem.highlighted ? "#ffffff" : "#21be2b"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: menuItem.highlighted ? "#21be2b" : "transparent"
                }
            }

            Menu{
                id: qsgshow
                title: "updateModel"
                property var gui: {
                    "title": "select images",
                    "content": {
                        "image1": {
                            value: "",
                            trig: "selectQSGImage"
                        },
                        "image2": {
                            value: "",
                            trig: "selectQSGImage"
                        }
                    }
                }

                Action {
                    text: "model"
                    shortcut: "Ctrl+S"
                    onTriggered: {
                        Pipelines().run("_setParam", qsgshow.gui, "testQSGModel")
                        //Pipelines().run("testQSGModel", view_cfg)
                    }
                }
                Action {
                    text: "face"
                    checkable: true
                    shortcut: "Ctrl+F"
                    onTriggered: {
                        view_cfg["face"] = 100 - view_cfg["face"]
                        Pipelines().run("_setParam", qsgshow.gui, "testQSGModel")
                       // Pipelines().run("testQSGModel", view_cfg)
                    }
                }
                Action {
                    text: "arrow"
                    checkable: true
                    shortcut: "Ctrl+A"
                    onTriggered: {
                        view_cfg["arrow"]["visible"] = !view_cfg["arrow"]["visible"]
                        Pipelines().run("_setParam", qsgshow.gui, "testQSGModel")
                        //Pipelines().run("testQSGModel", view_cfg)
                    }
                }
                Action {
                    text: "text"
                    checkable: true
                    shortcut: "Ctrl+T"
                    onTriggered: {
                        view_cfg["text"]["visible"] = !view_cfg["text"]["visible"]
                        Pipelines().run("_setParam", qsgshow.gui, "testQSGModel")
                        //Pipelines().run("testQSGModel", view_cfg)
                    }
                }
                Action {
                    text: "fps"
                    checkable: true
                    onTriggered: {
                        Pipelines().run("testFPS", {})
                    }
                }
                Component.onCompleted: {
                    Pipelines().add(function(aInput){
                        var pths = Pipelines().input({folder: false, filter: ["Image files (*.jpg *.png *.jpeg *.bmp)"]}, aInput.tag()).asyncCall("_selectFile").data()
                        if (pths.length)
                            aInput.setData(pths[0]).out()
                        else
                            aInput.setData("").out()
                    }, {name: "selectQSGImage"})

                    Pipelines().find("_setParam").nextF(function(aInput){
                        var dt = aInput.data()
                        if (dt["image1"]){
                            gui["content"]["image1"]["value"] = dt["image1"]
                            aInput.scope().cache("image1", dt["image1"])
                        }
                        if (dt["image2"]){
                            gui["content"]["image2"]["value"] = dt["image2"]
                            aInput.scope().cache("image2", dt["image2"])
                        }
                        aInput.outs(view_cfg, "testQSGModel")
                    }, "testQSGModel")
                }
            }

            Menu{
                title: "updateWholeAttr"

                MenuItem{
                    checkable: true
                    text: "wholeArrowVisible"
                    onClicked:{
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["arrow", "visible"], val: checked}], "wholeArrowVisible")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "wholeArrowPole"
                    onClicked:{
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["arrow", "pole"], val: checked}], "wholeArrowPole")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "wholeFaceOpacity"
                    onClicked: {
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["face"], val: checked ? 200 : 0}], "wholeFaceOpacity")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "wholeTextVisible"
                    onClicked: {
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["text", "visible"], val: checked}], "wholeTextVisible")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "wholeColor"
                    onClicked: {
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["color"], val: checked ? "yellow" : "green"}], "wholeColor")
                    }
                }

                MenuItem{
                    checkable: true
                    text: "wholeObjects"
                    onClicked: {
                        checked != checked
                        Pipelines().run("qml_updateQSGAttr_testbrd", [{key: ["objects"], type: checked ? "add" : "del", tar: "shp_3", val: {
                                                                                                         type: "poly",
                                                                                                         points: [[500, 300, 700, 300, 700, 500, 500, 300]],
                                                                                                         color: "pink",
                                                                                                         caption: "new_obj",
                                                                                                         face: 200
                                                                                                     }}], "wholeObjects")
                    }
                }

                Component.onCompleted: {
                    Pipelines().find("qml_QSGAttrUpdated_testbrd")
                    .nextF(function(aInput){
                        test_sum++
                        aInput.setData("Pass: qml_updateQSGAttr").out()
                    })
                    .next("testSuccessQML")
                }
            }
            Menu{
                title: "updateLocalAttr"

                MenuItem{
                    checkable: true
                    text: "polyArrowVisible"
                    onClicked:{
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["arrow", "visible"], val: checked}], "polyArrowVisible")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyArrowPole"
                    onClicked:{
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["arrow", "pole"], val: checked}], "polyArrowPole")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyFaceOpacity"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["face"], val: checked ? 200 : 0}], "polyFaceOpacity")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyTextVisible"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["text", "visible"], val: checked}], "polyTextVisible")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyColor"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["color"], val: checked ? "yellow" : "green"}], "polyColor")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyCaption"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["caption"], val: checked ? "poly_new" : "poly"}], "polyCaption")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyWidth"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["width"], val: checked ? 0 : 10}], "polyWidth")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyPoints"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["points"], val: checked ? [[50, 50, 200, 50, 200, 200, 50, 200, 50, 50], [80, 70, 120, 100, 120, 70, 80, 70]] : [[50, 50, 200, 200, 200, 50, 50, 50]]}], "polyPoints")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyStyle"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["style"], val: checked ? "dash" : "solid"}], "polyStyle")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "polyAngle"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_0", key: ["angle"], val: checked ? 45 : 0}], "polyAngle")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "imageAngle"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "img_2", key: ["angle"], val: checked ? 45 : 0}], "imageAngle")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "ellipseAngle"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_1", key: ["angle"], val: checked ? 90 : 20}], "ellipseAngle")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "ellipseCenter"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_1", key: ["center"], val: checked ? [600, 400] : [400, 400]}], "ellipseCenter")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "ellipseRadius"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_1", key: ["radius"], val: checked ? [200, 400] : [300, 200]}], "ellipseRadius")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "ellipseCCW"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "shp_1", key: ["ccw"], val: checked}], "ellipseCCW")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "imagePath"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "img_2", key: ["path"], val: checked ? "F:/3M/B4DT/DF Mark/V1-1.bmp" : "F:/3M/B4DT/DF Mark/V1-2.bmp"}], "imagePath")
                    }
                }
                MenuItem{
                    checkable: true
                    text: "imageRange"
                    onClicked: {
                        checked != checked
                        Pipelines().run("updateQSGAttr_testbrd", [{obj: "img_2", key: ["range"], val: checked ? [0, 0, 600, 800] : [0, 0, 400, 300]}], "imageRange")
                    }
                }
            }
        }

        Menu{
            title: "gui"
            Menu{
                title: "log"
                MenuItem{
                    text: "addLogRecord"
                    onClicked:{
                        Pipelines().run("addLogRecord", {type: "train", level: "info", msg: "train_info"})
                        Pipelines().run("addLogRecord", {type: "train", level: "warning", msg: "train_warning"})
                        Pipelines().run("addLogRecord", {type: "train", level: "error", msg: "train_error"})
                        Pipelines().run("addLogRecord", {type: "system", level: "info", msg: "system_info"})
                        Pipelines().run("addLogRecord", {type: "system", level: "warning", msg: "system_warning"})
                        Pipelines().run("addLogRecord", {type: "system", level: "error", msg: "system_error"})
                    }
                }
                MenuItem{
                    text: "showLogPanel"
                    onClicked:{
                        Pipelines().run("showLogPanel", {})
                    }
                }
            }
            Menu{
                title: "list"
                MenuItem{
                    text: "updateListView"
                    onClicked: {
                        Pipelines().run("_updateListView", {title: ["cat", "dog", "sheep", "rat"],
                                                          selects: [1, 3, 5],
                                                          data: [
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]}
                                                          ]})
                    }
                }
                MenuItem{
                    text: "modifyListView"
                    onClicked: {
                        Pipelines().run("_updateListView", {index: [2, 4, 5],
                                                          fontclr: "red",
                                                          data: [
                                                            {entry: [1, 3, 2, 3]},
                                                            {},
                                                            {entry: [2, 3, 2, 3]}
                                                          ]})
                    }
                }
                MenuItem{
                    text: "updatePageListView"
                    onClicked: {
                        Pipelines().run("pageList_updateListView", {title: ["idx", "dog", "sheep", "rat"],
                                                          selects: [1, 3, 11],
                                                          entrycount: 3,
                                                          pageindex: 2,
                                                          data: [
                                                            {entry: [0, 6, 2, 3]},
                                                            {entry: [1, 6, 2, 3]},
                                                            {entry: [2, 6, 2, 3]},
                                                            {entry: [3, 6, 2, 3]},
                                                            {entry: [4, 6, 2, 3]},
                                                            {entry: [5, 6, 2, 3]},
                                                            {entry: [6, 6, 2, 3]},
                                                            {entry: [7, 6, 2, 3]},
                                                            {entry: [8, 6, 2, 3]},
                                                            {entry: [9, 6, 2, 3]},
                                                            {entry: [10, 6, 2, 3]},
                                                            {entry: [11, 6, 2, 3]}
                                                          ]})
                    }
                }

                Component.onCompleted: {
                    Pipelines().find("_listViewSelected")
                    .nextF(function(aInput){
                        console.log(aInput.data())
                    }, "manual")
                    Pipelines().find("pageList_listViewSelected")
                    .nextF(function(aInput){
                        console.log(aInput.data())
                    }, "manual")
                }
            }

            Menu{
                title: "container"
                MenuItem{
                    text: "TWindow"
                    onClicked: {
                        twin.show()
                    }
                }
                MenuItem{
                    text: "swipe"
                    onClicked: swipe.show()
                }
                MenuItem{
                    text: "gridder"
                    onClicked: gridder.show()
                }
                MenuItem{
                    text: "flip"
                   // onClicked: flip.show()
                }
                MenuItem{
                    text: "nest"
                    onClicked:
                        nest.show()
                }
                MenuItem{
                    property var val: {
                        "spin_dflt": 1,
                        "spin": 2,
                        "check_dflt": true,
                        "check": true,
                        "edit_dflt": "dflt",
                        "edit": "edit",
                        "combo": 1,
                        "comboe": "sel3"
                    }

                    text: "PWindow"
                    onClicked: {
                        var cnt = {
                            spin_dflt: {
                            },
                            spin: {
                                type: "spin",
                                caption: "spin_"
                            },
                            check_dflt: {
                            },
                            check: {
                                type: "check",
                                caption: "check_"
                            },
                            edit_dflt: {
                            },
                            edit: {
                                type: "edit",
                                caption: "edit_",
                                trig: "getXXXParam"
                            },
                            combo: {
                                type: "combo",
                                model: ["sel1", "sel2"],
                                proto: ["key1", "key2"],
                                caption: "combo_"
                            },
                            comboe: {
                                type: "comboe",
                                model: ["sel1", "sel2"],
                                caption: "comboe_"
                            }
                        }
                        for (var i in cnt)
                            cnt[i]["value"] = val[i]
                        Pipelines().run("_setParam", {
                                                title: "set Param",
                                                content: cnt
                                            }, "manual")
                    }
                    Component.onCompleted: {
                        Pipelines().add(function(aInput){
                            aInput.setData("XXXParam").out()
                        }, {name: "getXXXParam"})
                        Pipelines().find("_setParam").nextF(function(aInput){
                            var dt = aInput.data()
                            for (var i in val)
                                if (dt[i] !== undefined)
                                    val[i] = dt[i]
                        }, "manual")
                    }
                }
                MenuItem{
                    text: "webwidget"
                    onClicked: Pipelines().run("openWebWindow", 0)
                }
                MenuItem{
                    text: "webqml"
                    onClicked: web.show()
                }
            }

            Menu{
                title: "dialog"

                Menu{
                    title: "file"
                    MenuItem{
                        text: "files"
                        onClicked: {
                            Pipelines().run("_selectFile", {folder: false, filter: ["Image files (*.jpg *.png *.jpeg *.bmp)"]}, "manual", {hello: "world"})
                        }
                    }
                    MenuItem{
                        text: "directory"
                        onClicked: {
                            Pipelines().run("_selectFile", {folder: true}, "manual2", {hello: "world"})
                        }
                    }
                    Component.onCompleted: {
                        Pipelines().find("_selectFile")
                        .nextF(function(aInput){
                            console.assert(aInput.scope().data("hello") === "world")
                            test_sum++
                            aInput.outs("Pass: _selectFile", "testSuccessQML")
                        }, "manual")
                        Pipelines().find("_selectFile")
                        .nextF(function(aInput){
                            console.assert(aInput.scope().data("hello") === "world")
                            test_sum++
                            aInput.outs("Pass: _selectFolder", "testSuccessQML")
                        }, "manual2")
                    }
                }

                MenuItem{
                    text: "color"
                    onClicked:
                        Pipelines().run("_selectColor", {}, "manual2", {hello: "world"})
                    Component.onCompleted: {
                        Pipelines().find("_selectColor")
                        .nextF(function(aInput){
                            console.assert(aInput.scope().data("hello") === "world")
                            test_sum++
                            aInput.outs("Pass: _selectColor", "testSuccessQML")
                        }, "manual2")
                    }
                }

                MenuItem{
                    text: "MsgDialog"
                    onClicked:
                        Pipelines().run("popMessage", {title: "hello4", text: "world"}, "manual", {hello: "world"})
                    Component.onCompleted: {
                        Pipelines().find("messagePoped")
                        .nextF(function(aInput){
                            console.assert(aInput.scope().data("hello") === "world")
                            test_sum++
                            aInput.outs("Pass: _popMessage", "testSuccessQML")
                        }, "manual")
                    }
                }

            }

            Menu{
                title: "diagram"

                MenuItem{
                    text: "linechart"
                    onClicked: {
                        linechart.show()
                    }
                }

                MenuItem{
                    text: "histogram"
                    onClicked: {
                        histogram.show()
                    }
                }

                MenuItem{
                    text: "thistogram"
                    onClicked: {
                        thistogram.show()
                    }
                }
            }

            MenuItem{
                text: "matrix"
                onClicked: {
                    matrix.show()
                }
            }

            MenuItem{
                text: "status"
                onClicked:
                    Pipelines().run("_updateStatus", ["hello", "world"])
            }
            Menu{
                title: "navigation"
                MenuItem{
                    text: "2"
                    onClicked: Pipelines().run("_updateNavigation", ["first layer", "second layer"], "manual")
                }
                MenuItem{
                    text: "3"
                    onClicked: Pipelines().run("_updateNavigation", ["first layer", "second layer", "third layer"], "manual")
                }
                MenuItem{
                    text: "4"
                    onClicked: Pipelines().run("_updateNavigation", ["first layer", "second layer", "third layer", "forth layer"], "manual")
                }
                Component.onCompleted: {
                    Pipelines().find("_updateNavigation")
                    .nextF(function(aInput){
                        test_sum++
                        aInput.outs("Pass: _updateNavigation", "testSuccessQML")
                    }, "manual")
                }
            }
            MenuItem{
                text: "search"
                onClicked:
                    Pipelines().run("_Searched", "", "manual")
            }

            MenuItem{
                text: "baseCtrl"
                onClicked: {
                    basectrl.show()
                }
            }
            MenuItem{
                text: "treeNodeView"
                onClicked: {
                    treeview.show()
                }
            }
            MenuItem{
                text: "treeView0"
                onClicked: {
                    treeview0.show()
                }
            }

            MenuItem{
                property string tag: "testProgress"
                property int cnt: 0
                property double hope
                text: "progress"
                onClicked: {
                    if (cnt % 10 == 0){
                        hope = 0.0
                        Pipelines().run("updateProgress", {title: "demo: ", sum: 10}, tag)
                        hope = 0.1
                        Pipelines().run("updateProgress", {}, tag)
                        ++cnt
                    }else if (cnt % 10 == 1){
                        hope = 0.2
                        Pipelines().run("updateProgress", {}, tag)
                        ++cnt
                    }else if (cnt % 10 == 2){
                        hope = 0.9
                        Pipelines().run("updateProgress", {step: 7}, tag)
                        cnt += 7
                    }else if (cnt % 10 == 9){
                        hope = 1.0
                        Pipelines().run("updateProgress", {}, tag)
                        ++cnt
                    }
                }
                Component.onCompleted: {
                    Pipelines().find("updateProgress")
                    .nextF(function(aInput){
                        console.assert(aInput.data() === hope)
                    }, tag)
                }
            }

            MenuItem{
                text: "videoOutput"
                onClicked: {
                    vdooutput.show()
                }
            }
        }

        DynamicQML{
            name: "menu"
            onLoaded: function(aItem){
                mainmenu.addMenu(aItem)
            }
        }

        Component.onCompleted: {
            view_cfg = {
                face: 0,
                arrow: {
                    visible: false,
                    pole: true
                },
                text: {
                    visible: false,
                    location: "middle"
                }
            }
        }
    }

    contentData:
        Column{
            anchors.fill: parent
            Row{
                width: parent.width
                height: parent.height - 60
                Column{
                    property int del_size
                    width: parent.width * 0.3 + del_size
                    height: parent.height
                    Search {
                        text: "search"
                        width: parent.width
                        height: 30
                        prefix: "#"
                    }
                    Rectangle{
                        width: parent.width
                        height: (parent.height - 30) / 2
                        color: "white"
                        List{
                            anchors.fill: parent
                        }
                    }
                    Rectangle{
                        width: parent.width
                        height: (parent.height - 30) / 2
                        color: "white"
                        PageList{
                            name: "pageList"
                            anchors.fill: parent
                        }
                    }
                    Component.onCompleted: {
                        Pipelines().find("_Searched")
                        .nextF(function(aInput){
                            var dt = aInput.data()
                            console.assert(dt === "search")
                            test_sum++
                            aInput.outs("Pass: _Searched", "testSuccessQML")
                        }, "manual")
                    }
                }
                Sizable{

                }
                Column{
                    property int del_size
                    width: parent.width * 0.7 + del_size
                    height: parent.height
                    QSGBoard{
                        id: testbrd
                        name: "testbrd"
                        plugins: [{type: "transform"}]
                        width: parent.width
                        height: parent.height * 0.7
                        Component.onDestruction: {
                            beforeDestroy()
                        }

                        BoardMenu{

                        }
                    }

                    Log{
                        width: parent.width
                        height: parent.height * 0.3
                    }
                }
            }
            Navigation{
                width: parent.width
                height: 30
            }
            Status{
                width: parent.width
                height: 30
            }
        }

    TWindow{
        id: twin
        caption: "TWindow"
        content: Column{
            anchors.fill: parent
            Rectangle{
                width: parent.width
                height: parent.height
                color: "gray"
            }
        }
        titlebuttons: [{cap: "O", func: function(){console.log("hello")}},
                        {cap: "W", func: function(){console.log("world")}}]
        footbuttons: [{cap: "OK", func: function(){close()}},
                       {cap: "Cancel", func: function(){close()}}]
    }

    TWindow{
        id: basectrl
        caption: "baseCtrl"
        content: Rectangle{
            anchors.fill: parent
            color: "gray"
            Column{
                /*Repeater{
                    delegate: T{

                    }
                }*/
                id: clm
                leftPadding: 5
                Edit{
                    width: 180
                    caption.text: "attr1" + ":"
                    ratio: 0.4
                }
                Check{
                    width: 180
                    caption.text: "attribu2" + ":"
                    ratio: 0.4
                }
                Spin{
                    width: 180
                    caption.text: "attribute555" + ":"
                    ratio: 0.4
                }
                Combo{
                    width: 180
                    caption.text: "attribute3" + ":"
                    ratio: 0.4
                }
                ComboE{
                    width: 180
                    caption.text: "attributeeee6" + ":"
                    ratio: 0.4
                    combo.modellist: ['test1', 'test2', 'test3', 'test4']
                    combo.currentSelect: "test2"
                }
                Track{
                    width: 180
                    caption.text: "attri4" + ":"
                    ratio: 0.4
                    onIndexChanged: function(aIndex){
                        console.log("track index: " + aIndex)
                    }
                }
                Radio{
                    text: "attribute4"
                }
                AutoSize{

                }
            }
        }
        footbuttons: [{cap: "OK", func: function(){close()}}]
    }

    TWindow{
        id: gridder
        caption: "gridder"
        content: Gridder{
            id: gridder_cld
            property var invisible_tag: {"3": true, "4": true, "6": true}

            name: "demo"
            scale: {"layout": [2, 2]}
            com: Component{
                Rectangle{
                    property string name
                    property int index
                    width: parent.width / parent.columns
                    height: parent.height / parent.rows

                    color: "transparent"
                    border.color: "red"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: parent.index
                    }
                }
            }

            padding: parent.width * 0.05
            width: parent.width * 0.9
            height: parent.height * 0.9
        }
        footbuttons: [
            {
                cap: "6",
                func: function(){
                    Pipelines().run(gridder_cld.name + "_updateViewCount", {sum: 6})
                }
            },
            {
                cap: "10",
                func: function(){
                    gridder_cld.updateViewCount({sum: 10, invisible: gridder_cld.invisible_tag})
                }
            },
            {
                cap: "10",
                func: function(){
                    gridder_cld.updateViewCount({sum: 10, layout: [2, 5]})
                }
            },
            {
                cap: "5x5",
                func: function(){
                    Pipelines().run(gridder_cld.name + "_updateViewCount", {layout: [5, 5]})
                }
            },
            {
                cap: "invisible",
                func: function(){
                    if (gridder_cld.invisible_tag[3])
                        gridder_cld.invisible_tag = {}
                    else
                        gridder_cld.invisible_tag = {"3": true, "4": true, "6": true}
                    Pipelines().run(gridder_cld.name + "_updateViewCount", {invisible: gridder_cld.invisible_tag})
                }
            },

        ]
    }

    TWindow{
        id: swipe
        caption: "swipeview"
        content: TabView{
            anchors.fill: parent
            Tab{
                title: "Red"
                Rectangle{
                    color: "red"
                }
            }
            Tab{
                title: "Blue"
                Rectangle{
                    color: "blue"
                }
            }
            Tab{
                title: "Swipe"
                Swipe{

                }
            }
        }
    }

    /*TWindow{
        id: flip
        caption: "flipView"
        content: Flip{
            id: flipview
            anchors.fill: parent
            front: Rectangle{
                anchors.fill: parent
                color: "red"
                MouseArea{
                    anchors.fill: parent
                    onClicked: flipview.flipUp()
                }
            }
            back: Rectangle{
                anchors.fill: parent
                color: "blue"
                MouseArea{
                    anchors.fill: parent
                    onClicked: flipview.flipDown()
                }
            }
        }
    }*/

    TWindow{
        id: nest
        caption: "nestView"
        content: Nest{
            rows: 10
            columns: 10
            anchors.fill: parent
            size: [1, 2, 7, 8, 8, 2, 2, 8, 1, 10]
            Rectangle {
                color: "red"
            }
            Rectangle {
                color: "green"
            }
            Rectangle {
                color: "blue"
            }
            Rectangle {
                color: "yellow"
            }
            Rectangle {
                color: "black"
            }
        }
    }

    TWindow{
        id: matrix
        caption: "matrix"
        content: Matrix{
            id: mtx
            rowcap.text: "hello"
            colcap.text: "world"
            anchors.fill: parent
            content: [[1, 2], [3, 4], [5, 6]]
        }
        footbuttons: [
            {
                cap: "3x3",
                func: function(){
                    mtx.content = [[1, 2, 3], [4, 5, 6], [7, "8", 9]]
                    mtx.updateGUI(true)
                }
            },
            {
                cap: "2x2",
                func: function(){
                    mtx.content = [[1, 2], [3, 4]]
                    mtx.updateGUI(true)
                }
            },
            {
                cap: "5x4",
                func: function(){
                    Pipelines().run("_updateMatrix", {rowcap: "hello2",
                                                    colcap: "world2",
                                                    content: [[1, 2, 3, 4],
                                                              [5, 6, 7, 8],
                                                              [9, 10, 11, 12],
                                                              [13, 14, 15, 16],
                                                              [17, 18, 19, 20]]})
                }
            }

        ]
        Component.onCompleted: {
            Pipelines().find("_matrixSelected")
            .nextF(function(aInput){
                console.log("_matrixSelected: " + aInput.data())
            }, "manual")
        }
    }

    TWindow{
        id: treeview
        property var sample: {
            "hello": "world",
            "he": [true, false],
            "hi": {
                "hi1": "hi2",
                "ge": {
                    "too": 3,
                    "heww": {
                        "ll": [3, 3, 4],
                        "dd": "dddd",
                        "ff": false
                    }
                }
            },
            "hi20": true,
            "hello2": "world",
            "he2": [0, {"kao": "gege"}, 1],
            "hi2": [
                {"hello": "world"},
                {"no": [true, false]}
            ],
            "hi22": 3
        }

        caption: "treeview"
        content: Rectangle{
            color: "gray"
            anchors.fill: parent
            TreeNodeView{
                anchors.fill: parent
            }
        }
        footbuttons: [
            {cap: "modify", func: function(){
                Pipelines().run("modifyTreeViewGUI", {key: ["hi2", "1", "no", "1"], val: true}, "modifyTreeView")}},
            {cap: "add", func: function(){
                Pipelines().run("modifyTreeViewGUI", {key: ["hi2", "1", "no", "2"], type: "add", val: 14}, "modifyTreeView")}},
            {cap: "delete", func: function(){
                Pipelines().run("modifyTreeViewGUI", {key: ["hi"], type: "del"}, "modifyTreeView")}},
            {cap: "load", func: function(){
                Pipelines().run("loadTreeView", {data: sample}, "testTreeView")}},
            {cap: "save", func: function(){
                Pipelines().run("saveTreeView", {}, "testTreeView")}},
            {cap: "style", func: function(){
                Pipelines().run("saveTreeView", {}, "styleTreeView")}}
            ]

        function sameObject(aTarget, aRef){
            for (var i in aTarget)
                if (typeof aRef[i] === "object"){
                    if (!sameObject(aTarget[i], aRef[i])){
                        //console.log(i + ";" + aTarget[i] + ";" + aRef[i])
                        return false
                    }
                }else if (aTarget[i] !== aRef[i]){
                    //console.log(i + ";" + aTarget[i] + ";" + aRef[i])
                    return false
                }
            return true
        }

        Component.onCompleted: {

        }
    }

    TWindow{
        id: linechart
        caption: "lineChart"
        content: LineChart{
            anchors.fill: parent
        }
        footbuttons: [
            {
                cap: "test",
                func: function(){
                    Pipelines().run("_updateLineChart", [20, 30, 100, 125, 30, 10, 12, 30, 50])
                }
            }
        ]
    }

    TWindow{
        id: histogram
        caption: "histogram"
        content: Histogram{
            anchors.fill: parent
        }
        footbuttons: [
            {
                cap: "test",
                func: function(){
                    Pipelines().run("_updateHistogramGUI", {histogram: [40, 20, 15, 25, 14, 16, 13, 30]})
                }
            }

        ]
    }

    TWindow{
        id: web
        caption: "webview"
        content: WebEngineView {
            anchors.fill: parent
            id: webview
            //url: "file:/html/test.html"
            //url: "https://www.baidu.com"
            url: "file:/mnist-core/index.html"
            z: - 1
            Column{
                anchors.fill: parent
                spacing: height * 0.2
                z: 1
                Rectangle{
                    width: parent.width
                    height: parent.height * 0.1
                    color: "red"
                    z: 1
                }
               /* Rectangle{
                    width: parent.width * 0.2
                    height: parent.height
                    color: "blue"
                }*/
            }
            webChannel: WebChannel{
                id: webview_chn
                /*Component.onCompleted: {
                    var stm = Pipelines().asyncCall("pipelineJSObject", 0)
                    webview_chn.registerObject("Pipelinec++", stm.data())
                    //webview_chn.registerObject("Pipeline", stm.scope().data("pipeline"))
                    webview.url = "file:/html/test.html"
                }*/
            }
        }
    }

    TWindow{
        id: treeview0
        content: TreeView0{
            anchors.fill: parent
            imagePath: {
                "image": '../../resource/image.png',
                "text": '../../resource/text.png',
                "shape": '../../resource/shape.png'
            }
            selectWay: 'background'
            selectColor: 'blue'
            openWay: 'all'
            onSetCurrentSelect: function (select) {
                console.log("treeview0 selected: " + select)
            }
            Component.onCompleted: {
                var tmp = [{
                               "type": "folder",
                               "name": "folder0",
                               "id": "folder0",
                               "children": [{
                                       "type": "page",
                                       "name": "page0",
                                       "id": "page0",
                                       "children": [{
                                               "id": "id0",
                                               "type": "image",
                                               "name": "image0",
                                               "position": [],
                                               "comment": "",
                                               "source": "" //url
                                           }, {
                                               "id": "id1",
                                               "type": "text",
                                               "name": "text0",
                                               "position": [],
                                               "comment": "",
                                               "relative_position": [],
                                               "content": "",
                                               "size": 16,
                                               "color": "green",
                                               "bold": ""
                                           }, {
                                               "id": "id2",
                                               "type": "shape",
                                               "name": "text0",
                                               "position": [],
                                               "comment": "",
                                               "relative_position": [],
                                               "direction": {
                                                   "color": "green",
                                                   "border": {
                                                       "type": "line",
                                                       "color": "red"
                                                   },
                                                   "radius": 30
                                               }
                                           }]
                                   }, {
                                       "type": "folder",
                                       "name": "folder0",
                                       "id": "folder1",
                                       "children": [{
                                               "id": "id3",
                                               "type": "text",
                                               "name": "text0_0",
                                               "range": [0, 0, 50, 50],
                                               "content": ""
                                           }]
                                   }]
                           }]
                buildDefaultTree(tmp, 'folder1')
            }
        }
    }

    Video{
        id: vdooutput
    }

    TWindow{
        id: thistogram
        caption: "thistogram"
        content: Column{
            anchors.fill: parent
            spacing: 5
            THistogram{
                id: histo
                width: parent.width
                height: parent.height * 0.9
                //oneThreshold: true
            }
            Track{
                property var intervals: []
                property var histogramdata: []
                property double value: 0.1
                property int idx: 0
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 0.05
                width: parent.width * 0.8
                interval: 100
                caption.text: "Interval" + ":"
                ratio: 0.2

                function updateInterval(){
                    if (intervals.length > 1 && histogramdata.length > 1){
                        histo.drawHistoGram(histogramdata[idx])
                    }
                }

                onIndexChanged: function(aIndex){
                    idx = aIndex
                    updateInterval()
                }
                Component.onCompleted: {
                    Pipelines().add(function(aInput){
                        var dt = aInput.data()
                        intervals = []
                        histogramdata = []
                        interval = Object.keys(dt["histogram"]).length - 1
                        for (var i in dt["histogram"]){
                            intervals.push(i)
                            histogramdata.push(dt["histogram"][i])
                        }
                        updateInterval()
                        return {out: {}}
                    }, {name: "_updateTHistogramGUI"})
                }
            }

            Track{
                property double value: 0
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 0.05
                width: parent.width * 0.8
                interval: 100
                caption.text: "IOU" + ":"
                ratio: 0.2

                onIndexChanged: function(aIndex){
                    value = aIndex / 100
                    //thresholdChanged(maxthreshold.x, minthreshold.x)
                }
            }
        }
        footbuttons: [
            {
                cap: "test",
                func: function(){
                    Pipelines().run("_updateTHistogramGUI", {histogram: {
                                      5: [10, 20, 15, 30, 25],
                                      10: [10, 20, 15, 30, 25, 20, 21, 23, 42, 12],
                                      20: [10, 20, 15, 30, 25, 20, 21, 23, 42, 12, 12, 10, 20, 42, 30, 15, 25, 20, 21, 23]
                                  }})
                }
            }

        ]
    }

    Progress{

    }
    MsgDialog{

    }
    File{

    }
    ColorSelect{

    }
    PWindow{

    }
    Component.onCompleted: {
        Pipelines().add(function(aInput){
            var tg = aInput.tag()
            Pipelines().find("_fileSelected").nextF(function(aInput){
                aInput.out(tg)
            }, "manual", {name: "js_fileSelected",
                    type: "Partial",
                    external: "js",
                    replace: true})

            aInput.out("manual")
        }, {name: "js_selectFile",
            aftered: "_selectFile",
            type: "Delegate",
            delegate: "js_fileSelected",
            external: "js"})
    }

    Component.onDestruction:{
        Pipelines().run("unloadMain", 0)
    }
}
