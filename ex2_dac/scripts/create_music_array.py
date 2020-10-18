from pathlib import Path
import string

from numpy.lib.function_base import append

file_array = ["godfather_theme"]

for loop_nr in range(len(file_array)):

    f1 = open(str(Path(__file__).parent)+"/" + file_array[loop_nr] + "_og.txt", "r")

    text = f1.read()

    letters_lower = string.ascii_lowercase
    letters_upper = string.ascii_uppercase
    note_number = int()
    found_note = False
    final_song_array = ""
    final_array = ""
    max_len = 200
    max_number_of_colums = 20
    number_of_colums = 0
    note = ""
    song_length = 0
    song_array = []
    set_array = []
    line_array = []
    new_array = []
    solv_array = []
    number_of_notes_array = []
    number_of_notes = 0
    current_line = 0
    current_line_set = 0
    note_number = 0

    #find line
    for i in text:
        if i in letters_lower:
            line_array.append(i.upper() + str(note_number))
        elif i in letters_upper:
            line_array.append(i + "S" + str(note_number) )
        elif i == "-":
            line_array.append("0")

        if i.isdigit():
            if note_number == 0:
                pass
            else:
                note_number_temp = int(i)
                if note_number_temp >= note_number:
                    set_array.append(line_array)
                    song_array.append(set_array)
                    set_array = []
                    line_array = []
                else:
                    set_array.append(line_array)
                    line_array = []
            note_number = int(i)

    set_array.append(line_array)
    song_array.append(set_array)
    set_array = []
    line_array = []


    for i in range(len(song_array)):
        new_array = []
        for j in range(len(song_array[i])):
            new_array.append(song_array[i][j])
        for j in range(len(new_array[0])):
            solv_array = []
            found_note = False
            for k in range(len(new_array)):
                solv_array.append(new_array[k][j])
            for k in solv_array:
                if k == "0":
                    pass
                else:
                    found_note = True
                    final_song_array += " " + k + ","
                    number_of_notes += 1
                    number_of_colums += 1
                    break
            if found_note == False:
                final_song_array += " 0,"
                number_of_notes += 1
                number_of_colums += 1
            if number_of_colums > max_number_of_colums:
                final_song_array += "\n"
                number_of_colums = 0
            if number_of_notes>=max_len:
                break
        if number_of_notes>=max_len:
            break

    final_song_array += " 0"
    number_of_notes += 1


    final_array = "song_t " + file_array[loop_nr] + " =  {" + str(number_of_notes) + ", {" +final_song_array + "} };\n" 

    f2 = open(str(Path(__file__).parent)+"/" + file_array[loop_nr] + "_song.txt", "w")
    f2.write(final_array)
    f2.close()