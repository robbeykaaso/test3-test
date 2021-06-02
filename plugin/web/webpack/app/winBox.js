const rea = require("reajs")
const Winbox = require("winbox")

let opened = null
rea.pipelines().add(function(aInput){
    if (!opened)
        opened = new WinBox({
            title: "Tool Box",
            background: "gray",
            top: 50,
            height: "100%",
            width: 75,
            x: "right",
            mount: document.getElementById("toolbox"),
            onclose: e => {
                opened = null
            },
            class: [
                "no-min",
                "no-max",
                "no-full",
                "no-resize",
                "no-header"
            ]
        })
    else{
        opened.close()
        opened = null
    }
    aInput.setData(opened != null).out()
}, {name: "_Toolbox"})

rea.pipelines().find("js_QSGAttrUpdated_graphic_ide").nextF(function(aInput){
    if (aInput.scope().data("hasModel") && !aInput.data().length)
        if (!opened){
            aInput.outs({}, "_Toolbox")
        }
})