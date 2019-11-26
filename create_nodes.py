import sys
import shutil
import os
import re

arguments=int(sys.argv[1])

file_lists=["node","node.o","nodeclass.o","util.o","nodeserver.o","nodesync.o"]

for _ in os.listdir("."):
	if(re.search("^Node[0-9]+$",_)):
		num=int(_.split("Node")[1])
		if(num>=arguments):
			shutil.rmtree(_)
			continue

		for root, dirs, files in os.walk(_, topdown=False):
			for name in files:
				os.remove(os.path.join(root, name))


for i in range(arguments):
	folder_name="Node"+str(i)
	if not (os.path.isdir(os.path.join(os.getcwd(),folder_name))):
		os.mkdir(folder_name)

	for file in file_lists:
		src="ChordNode/"+file
		des=folder_name+"/"+file

		shutil.copyfile(src,des)

	command="chmod +x "+folder_name+"/node"
	os.system(command)
