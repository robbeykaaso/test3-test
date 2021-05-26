jquery = require('jquery')
require('jstree')
require('jstree/dist/themes/default/style.css')
//https://webcache.googleusercontent.com/search?q=cache:9JnMpTlbPRsJ:https://github.com/vakata/jstree/issues/1275+&cd=14&hl=zh-CN&ct=clnk&gl=cz

module.exports = function() {
    jquery('#jstree').jstree({ 'core' : {
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
          }
        ]
    } });
};