from FM_Gui_Master import RootGUI, ComGui
from FM_V0x_Serial_V1 import FM_Serial

RootMaster = RootGUI()
MySerial   = FM_Serial() 
ComMaster  = ComGui(RootMaster.root, MySerial)
RootMaster.root.mainloop()