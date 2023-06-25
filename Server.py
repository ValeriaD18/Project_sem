import string

def data_base(login, password):
    with open('DataBase.txt', 'a') as out:
        out.write('{}:{} \n'.format(login, password))


def sign_in(login, password):
    with open('DataBase.txt') as inp:
        for i in inp.readlines():
            login_f, password_f = i.strip().split(':')
            if login == login_f:
                if password == password_f:
                    flag = 1
                    return flag
                else:
                    print("Введен неверный пароль!")
                    return 0
        print("Данный пользователь не найден в системе")
        return 0

def check(login):
        sec_password = input("Введите секретный ключ: ")
        smeshenie = 12
        letters = string.ascii_lowercase
        with open('C:/Users/User/Desktop/Универ/Project_sem/Token/DataBase_2.txt', 'r') as inp:
            for i in inp.readlines():
                login_f, password_f = i.strip().split(':')
                if login_f == login:
                    password = ''
                    for i in password_f:
                        mesto = letters.find(i)
                        new_mesto = (26 - smeshenie + mesto) % 26
                        password += letters[new_mesto]
                    if sec_password == password:
                        print("Вы успешно вошли в систему!")
                        return 0
                    else:
                        print("Введен неверный секретный ключ!")
                        return 0




