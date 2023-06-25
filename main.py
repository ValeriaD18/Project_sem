from Server import data_base, sign_in, check

'''Уязвимости методов ОТР:
    - фишинговая атака
    - атака "Человек посередине"
    - метод "грубой силы"
    - отсутствие проверки длины пароля
    - несвоевременное обновление пароля
'''




if __name__ == "__main__":
    command = input("Чтобы зарегистрироваться нажмите 1, чтобы войти нажмите 2: ")
    if command == "1":
        dic = {}
        login = input("Введите логин: ")
        with open('DataBase.txt') as inp:
            for i in inp.readlines():
                login_f, password_f = i.strip().split(':')
                dic[login_f] = password_f
        while True:
            if login in dic.keys():
                login = input("Введенный логин занят, введите другой: ")
                continue
            else:
                break
        password = input("Введите пароль: ")
        password_2 = input("Введите пароль повторно: ")
        if password == password_2:
            dic[login] = password
            data_base(login,password)
            print("Вы успешно зарегистрировались!")
    if command == "2":
        login = input("Введите логин: ")
        password = input("Введите пароль: ")
        sec_dic ={}
        flag = sign_in(login, password)
        if flag == 1:
            check(login)
