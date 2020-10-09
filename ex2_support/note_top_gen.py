desired_notes = {
    'G4': 392.00,
    'C5': 523.25,
    'E5': 659.25,
    'G5': 783.99
}

note_tops = desired_notes

array_length = 111

for note in desired_notes:
    FS = desired_notes[note]*array_length # frequency * array length
    note_tops[note] = round(14000000/FS)-1

#print("TOP =",top,14000000/(top+1)/111,"Hz, ", top-1,":", 14000000/(top)/111, ", ", top+1,":", 14000000/(top + 2)/111)
print(note_tops)