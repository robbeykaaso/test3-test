global.jQuery = require('jquery')
require('jstree')
require('jstree/dist/themes/default/style.css')
require("./style.css")
//require('bootstrap/dist/css/bootstrap.min.css')
var rea = require('reajs')
//https://webcache.googleusercontent.com/search?q=cache:9JnMpTlbPRsJ:https://github.com/vakata/jstree/issues/1275+&cd=14&hl=zh-CN&ct=clnk&gl=cz

module.exports = async function() {
  let dir = await rea.pipelines().input({folder: true}, "", null, true).asyncCall("js_selectFile")
  if (!dir.data().length)
      return
  let ret = await rea.pipelines().input(dir.data()[0], "", null, true).asyncCall("js_listFiles")
  let pths = ret.scope().data("data")

  let tr = jQuery("#jstree")
  
  tr.data("jstree", false).empty() //https://www.cnblogs.com/hofmann/p/12844311.html
  tr.jstree({ 'core' : {
    'data' : pths
  } })
  tr.jstree(true).refresh()
 

  /*  jQuery('#jstree').jstree({ 'core' : {
        'data' : [
           'Simple root node',
           {
             'text' : 'Root node 2',
             'state' : {
               'opened' : true,
               'selected' : true
             },
             'children' : [
               { 'text' : 'Child 1' },
               'Child 2'
             ]
          },
          "a",
          "b",
          "c",
          "d"
        ]
    } });*/
};