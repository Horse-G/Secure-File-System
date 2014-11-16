----user name---|---password---|----------------note----------------
     tinyvm     |   security   |   system file owner, non-previleged
	 u1     |   security1  |   normal user,non-previleged
	 u2     |   security2  |   intruder,non-previleged
	 
system file objput and objsetacl are set as setuid program as they involve writing previlege which was not granted to a normal executable binary.
As the principle of "least previlege" is adapted, the system only grant previlege to as few files as possible to minimize the possibility of system screw up by potential compromised use.