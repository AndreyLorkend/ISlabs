import sys
import os
import gostcrypto
from PySide2.QtWidgets import QApplication, QFileDialog, QMessageBox
from PySide2.QtUiTools import loadUiType

current_dir = os.path.dirname(os.path.abspath(__file__))
Form, Base = loadUiType(os.path.join(current_dir, "form.ui"))


class MainWindow(Base, Form):
    def __init__(self, parent=None):
        super(self.__class__, self).__init__(parent)
        self.setupUi(self)
        self.image = None
        self.choose_file_btn.clicked.connect(lambda: self.choose_file())
        self.generate_hash_btn.clicked.connect(lambda: self.generate_hash())

    def choose_file(self):
        filePath = QFileDialog.getOpenFileName(self, 'Open file', '')[0]
        self.file_path_textbox.setText(str(filePath))

    def generate_hash(self):
        filePath = str(self.file_path_textbox.text())
        buffer_size = 128
        hash_result = ""
        hash_obj = gostcrypto.gosthash.new('streebog256')
        if len(filePath):
            with open(filePath, 'rb') as file:
                buffer = file.read(buffer_size)
                while len(buffer) > 0:
                    hash_obj.update(buffer)
                    buffer = file.read(buffer_size)
            hash_result = hash_obj.hexdigest()
            self.hash_textbox.setText(str(hash_result))
        else:
             self.missing_file_error()

    def missing_file_error(self):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Critical)
        msg.setText("Отсутствует файл")
        msg.setWindowTitle("Ошибка")
        msg.exec_()


if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()
    widget.setWindowTitle("Хеш функция ГОСТ 34.11 Стрибог")
    sys.exit(app.exec_())
