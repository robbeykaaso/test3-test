import sys
from PyQt5.QtWidgets import QApplication, QWidget
from server import normalServer
import test

def main():

    app = QApplication(sys.argv)
    server = normalServer()
    #test.doTest(1)

    # w = QWidget()
    # w.resize(250, 150)
    # w.move(300, 300)
    # w.setWindowTitle('Simple')
    # w.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()