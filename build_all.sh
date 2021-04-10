set -x
circle meta1.cxx && ./meta1
circle meta2.cxx && ./meta2
circle meta3.cxx && ./meta3
circle meta4.cxx && ./meta4
circle meta5.cxx && ./meta5
circle meta6.cxx && ./meta6

circle commit1.cxx && ./commit1
circle commit2.cxx && ./commit2

circle sort.cxx && ./sort
circle call.cxx && ./call
circle monster.cxx && ./monster
circle monster2.cxx && ./monster2