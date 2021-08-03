import sys
from PyQt5.QtWidgets import QApplication, QWidget
from reapython.server import normalServer
from reapython.rea import pipelines, pipeline, stream
from reapython.reaRemote import connectRemote, pipelineRemote
#import test

pipelines().add(lambda aInput:
    aInput.setData(pipeline("server")).out()
, {"name": "createserverpipeline"})

pipelines().add(lambda aInput:
    aInput.setData(pipelineRemote("c++", "server")).out()
, {"name": "createc++pipeline"})

pipelines().add(lambda aInput:
    aInput.setData(pipelineRemote("qml", "server")).out()
, {"name": "createqmlpipeline"})

pipelines("server").add(lambda aInput: 
    aInput.out()
, {"name": "logTrain",
   "external": "qml"})

def clientOnline(aInput: stream):
    aInput.outs("hello", "logTrain")
pipelines("server").add(clientOnline, {"name": "clientOnline", "external": "c++"})

def main():

    app = QApplication(sys.argv)

    server = normalServer()
    def writeRemote(aInput: stream):
        server.writeSocket(aInput.scope().data("socket"), aInput.data())
    connectRemote("server", "c++", writeRemote, False, "c++_server")
    connectRemote("server", "qml", writeRemote, False, "qml_server")
    #test.doTest(1)

    # w = QWidget()
    # w.resize(250, 150)
    # w.move(300, 300)
    # w.setWindowTitle('Simple')
    # w.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()