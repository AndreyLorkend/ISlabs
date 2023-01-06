# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys
import gostcrypto

from PySide2.QtWidgets import QApplication, QWidget, QMessageBox, QFileDialog
from PySide2.QtUiTools import loadUiType

current_dir = os.path.dirname(os.path.abspath(__file__))
Form, Base = loadUiType(os.path.join(current_dir, "form.ui"))


class MainWindow(Base, Form):
    def __init__(self, parent=None):
        super(self.__class__, self).__init__(parent)
        self.setupUi(self)
        self.image = None
        self.choose_file_btn.clicked.connect(lambda: self.choose_file())
        self.choose_sign_btn.clicked.connect(lambda: self.choose_sign_file())
        self.choose_pub_btn.clicked.connect(lambda: self.choose_pub_file())
        self.sign_btn.clicked.connect(lambda: self.sign_file())
        self.verify_btn.clicked.connect(lambda: self.verify_file())

    def generate_hash(self, filePath):
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
        return hash_result

    def sign(self, f_hash, private_key):
        digest = f_hash

        sign_obj = gostcrypto.gostsignature.new(gostcrypto.gostsignature.MODE_256,
                                                gostcrypto.gostsignature.CURVES_R_1323565_1_024_2019[
                                                    'id-tc26-gost-3410-2012-256-paramSetB'])

        signature = sign_obj.sign(bytearray.fromhex(private_key), bytearray.fromhex(digest))
        if signature:
            file = open('f_sign.sign', 'wb')
            file.write(signature)
            file.close()
            self.message_dialog("Успех", "Файл подписан", QMessageBox.Information)
        else:
            self.message_dialog("Ошибка", "Файл не подписан", QMessageBox.Critical)

    def verify(self, public_key, f_hash, f_sign):
        public_key = bytearray.fromhex(public_key)
        digest = bytearray.fromhex(f_hash)
        signature = bytearray.fromhex(f_sign)

        sign_obj = gostcrypto.gostsignature.new(gostcrypto.gostsignature.MODE_256,
                                                gostcrypto.gostsignature.CURVES_R_1323565_1_024_2019[
                                                    'id-tc26-gost-3410-2012-256-paramSetB'])
        if sign_obj.verify(public_key, digest, signature):
            self.message_dialog("Успех", "Подпись подтверждена", QMessageBox.Information)
        else:
            self.message_dialog("Ошибка", "Подпись не прошла проверку", QMessageBox.Critical)

    def generate_public_key(self, private_key_hex):
        private_key = bytearray.fromhex(private_key_hex)
        sign_obj = gostcrypto.gostsignature.new(gostcrypto.gostsignature.MODE_256,
                                                gostcrypto.gostsignature.CURVES_R_1323565_1_024_2019[
                                                    'id-tc26-gost-3410-2012-256-paramSetB'])

        public_key = sign_obj.public_key_generate(private_key)
        file = open('public_key.pub', 'wb')
        file.write(public_key)
        file.close()

    def generate_private_key(self):
        private_key = gostcrypto.gostrandom.new(32, size_s=gostcrypto.gostrandom.SIZE_S_256)
        file = open('private_key.pr', 'wb')
        key = private_key.random()
        file.write(key)
        file.close()

    def message_dialog(self, title, message, icon):
        msg = QMessageBox()
        msg.setIcon(icon)
        msg.setText(message)
        msg.setWindowTitle(title)
        msg.exec_()

    def missing_file_error(self, error_text):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Critical)
        msg.setText(error_text)
        msg.setWindowTitle("Ошибка")
        msg.exec_()

    def choose_file(self):
        filePath = QFileDialog.getOpenFileName(self, 'Open file', '')[0]
        self.file_path_field.setText(str(filePath))

    def choose_sign_file(self):
        filePath = QFileDialog.getOpenFileName(self, 'Open signature', '', '(*.sign)')[0]
        self.sign_path_field.setText(str(filePath))

    def choose_pub_file(self):
        filePath = QFileDialog.getOpenFileName(self, 'Open public key', '', '(*.pub)')[0]
        self.pub_path_field.setText(str(filePath))

    def sign_file(self):
        self.generate_private_key()
        self.generate_public_key(open('private_key.pr', 'rb').read().hex())
        self.sign(self.generate_hash(self.file_path_field.text()), open('private_key.pr', 'rb').read().hex())

    def verify_file(self):
        self.verify(open(self.pub_path_field.text(), 'rb').read().hex(),
                            self.generate_hash(self.file_path_field.text()),
                                open(self.sign_path_field.text(), 'rb').read().hex())


if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()
    widget.setWindowTitle("Хеш функция ГОСТ 34.11 Стрибог")
    sys.exit(app.exec_())
