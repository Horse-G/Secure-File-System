build:
	
	gcc objget.c -o objget
	gcc objput.c -o objput
	gcc objlist.c -o objlist
	gcc objgetacl.c -o objgetacl
	gcc objsetacl.c -o objsetacl
	gcc objtestacl.c -o objtestacl
	chmod u+s objput
	chmod u+s objsetacl
	
	
objectstore: objget.c objput.c objlist.c objgetacl.c objsetacl.c objtestacl.	
	
test: build
	@echo "------begin test-------"
	@echo "------------------------------add obj:-------"
	@echo "case 1: normal case -success"
	su u1 -c "./objput doc testfile.txt"
	@echo "case 2: denied case -illegeal filename"
	- su u1 -c "./objput -u u1 -g g1 doc#$% testfile.txt"
	
	@echo "------------------------------get obj:-------"
	@echo "case 1: normal case -allowed"
	su u1 -c "./objget doc"
	@echo "case 2: denied case -wrong user/group"
	- su u2 -c "./objget doc"
	@echo "case 3: denied case -no such file"
	- su u1 -c "./objget doc2"
	
	@echo "------------------------------list obj:------"
	@echo "*********adding testing data************"
	su u1 -c "./objput doc2 testfile.txt"
	su u1 -c "./objput doc3 testfile.txt"
	-su u2 -c "./objput doc1 testfile.txt"
	-su u2 -c "./objput doc4 testfile.txt"
	@echo "*********finish adding testing data*****"
	@echo "case 1: normal list without -l"
	su u1 -c "./objlist"
	@echo "case 2: normal list with -l"
	-su u2 -c "./objlist -l"
	
	@echo "------------------------------get acl:-------"
	@echo "case 1:normal case -allowed"
	su u1 -c "./objgetacl doc"
	@echo "case 2:denied case -user file not matching"
	- su u2 -c "./objgetacl doc"
	@echo "case 3:denied case -user have no 'v' permission"
	@echo "*********adding testing data************"
	su u1 -c "./objsetacl doc2 test.acl"
	@echo "*********finish adding testing data*****"
	- su u1 -c "./objgetacl doc2"
	
	@echo "------------------------------set acl:-------"
	@echo "case 1:normal case -allowed"
	su u1 -c "./objsetacl doc test.acl"
	@echo "case 2:denied case-user file not matching"
	-su u2 -c "./objsetacl doc test.acl"
	@echo "case 3:denied case -user have no 'p' permission"
	- su u1 -c "./objsetacl doc2 test.acl"
	
	@echo "------------------------------test acl:------"
	@echo "case 1:normal case:allowed"
	su u1 -c "./objtestacl -r r doc"
	@echo "case 2:denied case -user file not matching"
	-su u2 -c "./objtestacl -r r doc1"
	@echo "case 3:denied case -user have no permission"
	- su u1 -c "./objtestacl -r w doc"
	@echo "-------end of test-------"
	
test1:
	-echo security2 | -S su u2 -c "./objtestacl -r r doc"
exec: build
	
	
.PHONY: clean

clean:
	-rm objget objput objlist objgetacl objsetacl objtestacl
	-rm *.doc*
	mv test.acl test
	- rm *.acl default u* 
	mv test test.acl
