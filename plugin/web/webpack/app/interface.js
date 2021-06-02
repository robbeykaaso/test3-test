const rea = require("reajs")

rea.pipelines().find("_Toolbox").nextF(function(aInput){
    aInput.out()
}, "", {name: "qml_Toolbox", external: "qml"})