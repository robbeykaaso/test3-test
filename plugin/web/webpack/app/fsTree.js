const rea = require('reajs')

require('jstree')
require('jstree/dist/themes/default/style.css')
//require('bootstrap/dist/css/bootstrap.min.css')
//https://webcache.googleusercontent.com/search?q=cache:9JnMpTlbPRsJ:https://github.com/vakata/jstree/issues/1275+&cd=14&hl=zh-CN&ct=clnk&gl=cz

let mdl = {
  "loaded": {},
  "id": ""
}

async function getTreeData2(aPathList){
  const org = {
    folder0: {
      file0: "text.json",
      folder0: {
        file0: "xxx.bmp",
        file1: "fff.txt",
        folder0: {
          file0: "ggg.txt"
        }
      }
    },
    folder1: {

    },
    folder2: {

    },
    file0: "adsfasf.xml"
  }
  let root = org
  if (aPathList)
    for (let i in aPathList)
      root = root[aPathList[i]]
  
  let ret = []
  for (let i in root)
    if (i.startsWith("folder"))
      ret.push(i)
    else if (i.startsWith("file"))
      ret.push(root[i])
  return ret
}

async function getTreeData(aPathList){
  let dir0 = ""
  if (!aPathList){
    let dir = await rea.pipelines().input({folder: true}, "", null, true).asyncCall("js_selectFile")
    if (!dir.data().length)
        return
    dir0 = dir.data()[0]
    mdl.id = dir0
  }else{
    for (let i in aPathList){
      if (dir0 != "")
        dir0 += "/"
      dir0 += aPathList[i]
    }
  }
  return (await rea.pipelines().input(dir0, "", null, true).asyncCall("js_listFiles")).scope().data("data")
}

const icons = {
  "png": "img",
  "bmp": "img",
  "jpg": "img",
  "jpeg": "img",
  "xml": "txt",
  "txt": "txt",
  "json": "txt"
}

function prepareEntry(aID, aText){
  let entry = {
    id: aID,
    text: aText
  }
  let ext = aText.split(".").pop()
  if (icons[ext]){
    entry["icon"] = icons[ext] + ".png"
  }/*else{
    entry["state"] = {
      opened: false
    }
  }*/
  return entry
}

function refreshTree(aData){
  let tr = $("#jstree")
  tr.data("jstree", false).empty() //https://www.cnblogs.com/hofmann/p/12844311.html
  tr.jstree({
    "core": {
      "check_callback": true,
      "data" : aData
    }})
  tr.jstree(true).refresh()
}

let tr = $("#jstree")
/*tr.on("open_node.jstree", function (e, data) {
  console.log("lala")
  //do something
});
tr.on("after_open.jstree", function (e, data) {
  console.log("lala3")
  //do something
});
tr.on("close_node.jstree", function (e, data) {
  console.log("lala2")
  //do something
});
tr.on("changed.jstree", function (e, data){
  //console.log(data.selected)
});

tr.on("ready.jstree", function(e, data){
// console.log(last_open)
// if (last_open)
//  tr.jstree().open_node(last_open)
  //tr.jstree()._open_to(last_open)

})*/
tr.on("dblclick.jstree", async function(e){
  let node = $(e.target).closest("li")
  if (mdl.loaded[node[0].id])
    return
  mdl.loaded[node[0].id] = true
  let ids = node[0].id.split("/")
  let dt = await getTreeData(ids)
  if (!dt.length){
    rea.pipelines().run("js_openWorkFile", node[0].id)
    return
  }
  
  //let prt = tr.jstree("get_selected")
  for (let i in dt){
    if (dt[i] != "." && dt[i] != ".."){
      let ent = prepareEntry(node[0].id + "/" + dt[i], dt[i])
      tr.jstree().create_node([node[0].id], ent)
    }
  }
  tr.jstree().open_node([node[0].id])
})

rea.pipelines().add(async function(){
  let dt = await getTreeData()
  let tree_dt = []
  for (let i in dt)
    if (dt[i] != "." && dt[i] != "..")
      tree_dt.push(prepareEntry((mdl.id == "" ? "" : mdl.id + "/") + dt[i], dt[i]))

  refreshTree(tree_dt) 
}, {name: "Open_Folder"})