with open("ex3_full_game\\local_src\\game-1.0\\testing.txt",newline='') as a:
    t = a.read()
    for i in t:
        try:
            print("{0:b}".format(int(i)))
        except:
            pass