import tkinter as tk
import os
import subprocess

os.system("cls")
command = "g++ Project.cpp -o main.exe && main.exe"
subprocess.run(command, shell=True)

row_size, column_size, num_ball, num_hole, show_mod, carry = 0, 0, 0, 0, 0, 0
beginning_location = (0, 0)
cell = [([0] * 5) for _ in range(5)]
Ant = [(0, 0) for _ in range(4)] 
agent = (0, 0)


def draw_colored_table(rows, cols, cell_size):
    global carry
    root = tk.Tk()
    canvas = tk.Canvas(root, width=cols*cell_size, height=rows*cell_size)
    canvas.pack()


    def temp(i, j):
        x0, y0 = j * cell_size, i * cell_size
        x1, y1 = x0 + cell_size, y0 + cell_size
        if ((i + j) % 2 == 0):
            color = "white"
        else:
            color = "gray"
        canvas.create_rectangle(x0, y0, x1, y1, fill=color)

    def start(i, j):
        if(cell[i][j] == 4):
            for ii in range(4):
                for jj in range(4):
                    y0, x0 = i * cell_size + ii *  cell_size / 4, j * cell_size + jj *  cell_size / 4
                    y1, x1 = y0 + cell_size / 4, x0 + cell_size / 4
                    if((ii + jj) % 2 == 0):
                        canvas.create_rectangle(x0, y0, x1, y1, fill="yellow")
                    else:
                        canvas.create_rectangle(x0, y0, x1, y1, fill="green")
    
    def red_black(i, j, mod):
        x0, y0 = j * cell_size, i * cell_size
        x0, y0 = x0 + cell_size * 0, y0 + cell_size * 0
        x1, y1 = x0 + cell_size * 1, y0 + cell_size * 1
        if (cell[i][j] == 2):
            canvas.create_oval(x0, y0, x1, y1, fill="red")
        elif (cell[i][j] == 3):
            canvas.create_oval(x0, y0, x1, y1, fill="black")
        if(mod == 1):
            cell[i][j] = 0
    
    def red(i, j):
        if carry == 1 and agent[0] == i and agent[1] == j:
            x0, y0 = j * cell_size, i * cell_size
            x0, y0 = x0 + cell_size * 0, y0 + cell_size * 0
            x1, y1 = x0 + cell_size * 1, y0 + cell_size * 1
            canvas.create_oval(x0, y0, x1, y1, fill="red")
    
    def blue():
        for a in range(4):
            y0, x0 = Ant[a][0] * cell_size + int(a / 2) *  cell_size / 2, Ant[a][1] * cell_size + (a % 2) *  cell_size / 2
            y1, x1 = y0 + cell_size / 4, x0 + cell_size / 4
            canvas.create_oval(x0, y0, x1, y1, fill="blue")
        
    def green(i, j):
        x0, y0 = j * cell_size, i * cell_size
        x0, y0 = x0 + cell_size * 0, y0 + cell_size * 0
        x1, y1 = x0 + cell_size * 1, y0 + cell_size * 1
        canvas.create_oval(x0, y0, x1, y1, fill="green")
        cell[i][j] = 5
   
    # Making Table
    for i in range(rows):
        for j in range(cols):
            temp(i, j)
            start(i, j)
            red_black(i, j, 0)
            red(i, j)
            if cell[i][j] == 5:
                green(i, j)
    
    i, j = agent[0], agent[1]
    # Ant
    if show_mod == 1:
        blue()
    # Agent
    elif show_mod == 2:
        if carry == 1:
            if cell[i][j] == 0 or cell[i][j] == 4 or cell[i][j] == 5:
                red_black(i, j, 0)
            elif cell[i][j] == 3:
                green(i, j)
                carry = 0                
        elif cell[i][j] == 2:
            red_black(i, j, 1)
            carry = 1
        x0, y0 = j * cell_size + cell_size * 0.2, i * cell_size + cell_size * 0.2
        x1, y1 = x0 + cell_size * 0.6, y0 + cell_size * 0.6
        canvas.create_rectangle(x0, y0, x1, y1, fill="purple")

    def close_window():
        root.destroy()
    if show_mod == 1:  
        root.after(1000, close_window)  
        root.mainloop()
    elif show_mod == 2:
        root.after(2000, close_window)  
        root.mainloop()
    else:
        root.after(5000, close_window)  
    root.mainloop()

with open("output.txt", 'r') as file:
    counter_line = 0
    lines = file.readlines()
    for line in lines:
        if line.strip():  
            num = line.strip().split()
            len_num = len(num)
            if counter_line == 0:
                row_size = int(num[0])
                column_size = int(num[1])
                num_ball = int(num[2])
                num_hole = int(num[3])
            elif counter_line == 1:
                beginning_location = (int(num[0]), int(num[1]))
                cell[int(num[0])][int(num[1])] = 4
            elif counter_line >= 2 and counter_line < 2 + num_ball:
                cell[int(num[0])][int(num[1])] = 2
            elif counter_line >= 2 + num_ball and counter_line < 2 + num_ball + num_hole:
                cell[int(num[0])][int(num[1])] = 3
            else:
                if show_mod == 0:
                    draw_colored_table(row_size, column_size, 160)
                    show_mod = 1
                    for i in range(4):
                        Ant[i] = beginning_location
                if int(num[0]) < 4:  
                    Ant[int(num[0])] = (int(num[1]), int(num[2]))  
                    # draw_colored_table(row_size, column_size, 160)
                    if((1000 - int(num[3])) % 25 == 0):
                        os.system("cls")
                        print('\n\n\tTraining:\t', (1000 - int(num[3])) / 10, '%')
                elif show_mod == 1:
                    show_mod = 2
                if(show_mod == 2):
                    agent = (int(num[1]), int(num[2]))  
                    draw_colored_table(row_size, column_size, 160)
        counter_line += 1
command = "del output.txt && del main.exe"
subprocess.run(command, shell=True)
