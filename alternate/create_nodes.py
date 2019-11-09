import sys
import shutil
import os
import glob

arguments=int(sys.argv[1])

file_lists=["node","node.o","nodeclass.o","util.o","nodeserver.o"]

for i in range(arguments):
	folder_name="Node"+str(i)
	
	try:
		for root, dirs, files in os.walk(folder_name):
			for file in files:
				os.remove(os.path.join(root, file))
	except Exception:
		os.mkdir(folder_name)

	for file in file_lists:
		src="ChordNode/"+file
		des=folder_name+"/"+file

		shutil.copyfile(src,des)

	command="chmod +x "+folder_name+"/node"
	os.system(command)