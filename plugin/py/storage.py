from PyQt5.QtCore import QByteArray, QJsonDocument, QFile, QDir
from reapython.rea import pipelines, stream
import json

class fsStorage:
    def __init__(self, aRoot: str = ""):
        self.__m_root = aRoot
        pass

    def stgRoot(self, aPath: str) -> str:
        if self.__m_root == "":
            return aPath
        else:
            return self.__m_root + "/" + aPath

    def checkPath(aPath: str):
        dirs = aPath.split("/")
        dir = QDir()
        origin = ""
        len_dirs = len(dirs)
        for i in range(len_dirs):
            if i > 0:
                origin += "/"
            origin += dirs[i]
            if not dir.exists(origin):
                dir.mkdir(origin)

    def initialize(self):
        def readJsonObject(aInput: stream):
            ret, dt = self.readJsonObject(aInput.scope().data("path"))
            aInput.scope().cache("data", dt)
            aInput.setData(ret).out()
        pipelines().add(readJsonObject, {"name": "readJsonObject", "thread": 10})

        def writeJsonObject(aInput: stream):
            ret = self.writeJsonObject(aInput.scope().data("path"), aInput.scope().data("data"))
            aInput.setData(ret).out()
        pipelines().add(writeJsonObject, {"name": "writeJsonObject", "thread": 11})

    def writeByteArray(self, aPath: str, aData: bytearray) -> bool:
        pth = self.stgRoot(aPath)
        self.checkPath(pth)
        fl = QFile(pth)
        if fl.open(QFile.WriteOnly):
            fl.write(aData)
            fl.close()
            return True
        return False

    def readJsonObject(self, aPath: str) -> tuple:
        fl = QFile(self.stgRoot(aPath))
        if fl.open(QFile.ReadOnly):
            doc = QJsonDocument.fromJson(fl.readAll())
            return True, doc.object()
        return False, None

    def writeJsonObject(self, aPath, aData: dict) -> bool:
        self.writeByteArray(aPath, bytearray(json.dumps(aData).encode("utf-8")))