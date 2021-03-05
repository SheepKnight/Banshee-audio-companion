import tkinter as tk
import arduino




col = 3 #Nombres de sliders


def updateStatus(text,color="black"):
    """Met a jour le texte d'etat"""

    lbl_status["text"] = text
    lbl_status["fg"] = color

    return None


def btnLed():
    """Ouvre une fenetre pour choisir de options LED"""
    leds = tk.Tk()
    leds.title("Gestion leds")

    btn_anim = {}
    noms_anim = ["Rainbow","Bass","Boite"]
    nbAnims = 3

    frm_led_main = tk.Frame(master=leds)
    frm_led_main.grid(row=1, column=1, pady=10)

    lbl_led = tk.Label(master=frm_led_main, text="Select animation")
    lbl_led.pack()

    frm_led_main.grid(row=2, column=1, pady=10)
    frm_led_buttons = tk.Frame(master=frm_led_main)
    frm_led_buttons.pack()

    for i in range(nbAnims):
        frm_led_1 = tk.Frame(master=frm_led_buttons)
        frm_led_1.grid(row = 1,column = i)
        btn_anim[i] = tk.Button(text = noms_anim[i],master = frm_led_1,width = 20,height =5)
        btn_anim[i].pack(padx=10,pady = 10)


    #updateStatus("L'animation Led à été changé")

    return None


def btnGet():
    """Recupere les données de la teensy et ajuste l'interface"""
    data = arduino.getData()

    for d in range(col) :
        sliders[i].set(data[d][0])
        prompts[i].delete(0, tk.END)
        prompts[i].insert(str(data[d][1]))

    updateStatus("All informations retieved","Orange")

    return None


def btnSet():
    """Envoit les données à la teensy"""
    data = []
    for i in range(col):
        data.append({"id": i, "delay":sliders[i]["delay"].get(), "bass":sliders[i]["bass"].get(), "mid":sliders[i]["mid"].get(), "high":sliders[i]["high"].get()})

    arduino.writeData(data)

    updateStatus("All informations sent","Green")

    return None


#Setup la fenetre
window = tk.Tk()
window.title("Banshee Multi-Speakers Companion")

#Frame principal
frm_main = tk.Frame(master=window)
frm_main.grid(row=1,column=1,pady=10)

#Texte du haut
label = tk.Label(master = frm_main,font=("TkDefaultFont",44),text="Banshee")
label.pack()


frm_main.grid(row=2,column=1,pady=10)


sliders = [{"delay":None, "bass":None, "mid":None, "high":None}, {"delay":None, "bass":None, "mid":None, "high":None}, {"delay":None, "bass":None, "mid":None, "high":None}]
prompts = [None, None, None]
labels = [None, None, None]
labelsTxt = ["Left","Center","Right"]

#frame pour les sliders
frm_2 = tk.Frame(master=frm_main)
frm_2.pack()

#créer les elements de reglage
for i in range(col):
    frm_1 = tk.Frame(master=frm_2,relief="sunken",borderwidth = 4)
    frm_1.grid(column=i, row = 1)
    lbl_2 = tk.Label(frm_1,text = labelsTxt[i],font = ("TkDefaultFont",20))
    lbl_2.pack(pady=5, padx =10)
    sliders[i]["delay"] = tk.Scale(master =frm_1,from_ = 100, to=0,length =200,tickinterval=10)
    sliders[i]["delay"].set(50)
    sliders[i]["delay"].pack(pady=5)
    lbl_2 = tk.Label(frm_1,text = "Delay :",font=("TkDefaultFont",12))
    lbl_2.pack(pady=5)
    prompts[i] = tk.Entry(master=frm_1,width=20)
    prompts[i].pack(padx=5,pady=5)
    sliders[i]["bass"] = tk.Scale(master =frm_1,from_ = 100, to=0,length =80,tickinterval=1)
    sliders[i]["bass"].set(50)
    sliders[i]["bass"].pack(pady=5)
    sliders[i]["mid"] = tk.Scale(master =frm_1,from_ = 100, to=0,length =80,tickinterval=1)
    sliders[i]["mid"].set(50)
    sliders[i]["mid"].pack(pady=5)
    sliders[i]["high"] = tk.Scale(master =frm_1,from_ = 100, to=0,length =80,tickinterval=1)
    sliders[i]["high"].set(50)
    sliders[i]["high"].pack(pady=5)


frm_main.grid(row=3,column=1,pady=10)

#Boutons setup/led
buttons = {}
buttons_txt = ["LEDs","Set","Get"]


frm_btn = tk.Frame(master=frm_main)
frm_btn.pack()


for i in range(3):

    frm_4 = tk.Frame(master=frm_btn)
    frm_4.grid(column=i,row = 1)

    buttons[i] = tk.Button(master = frm_4,text=buttons_txt[i],width = 25,height =5)
    buttons[i].pack(padx=10,pady=10)

buttons[0]["command"] = btnLed
buttons[1]["command"] = btnSet
buttons[2]["command"] = btnGet



#Label de status
frm_main.grid(row=4,column=1,pady=10)

lbl_status = tk.Label(master=frm_main,text="All Clear",fg = "green")
lbl_status.pack()

window.mainloop()
