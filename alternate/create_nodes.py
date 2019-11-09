import sys
import shutil
import os

arguments=int(sys.argv[1])

file_lists=["node","node.o","nodeclass.o"]

for i in range(arguments):
	folder_name="Node"+str(i)
	
	try:
		shutil.rmtree(folder_name, ignore_errors=True)
	except Exception:
		print("",end="")

	os.mkdir(folder_name)

	for file in file_lists:
		src="ChordNode/"+file
		des=folder_name+"/"+file

		shutil.copyfile(src,des)