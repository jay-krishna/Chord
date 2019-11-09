import sys
import shutil
import os
import re

arguments=int(sys.argv[1])

file_lists=["node","node.o","nodeclass.o","util.o","nodeserver.o"]

for _ in os.listdir("."):
	if(re.search("^Node[0-9]+$",_)):
		shutil.rmtree(os.path.join(os.getcwd(),_))

for i in range(arguments):
	folder_name="Node"+str(i)
	os.mkdir(folder_name)

	for file in file_lists:
		src="ChordNode/"+file
		des=folder_name+"/"+file

		shutil.copyfile(src,des)

	command="chmod +x "+folder_name+"/node"
	os.system(command)
