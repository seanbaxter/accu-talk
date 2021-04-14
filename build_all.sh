set -x

circle meta1.cxx && ./meta1

circle meta2.cxx && ./meta2

circle meta3.cxx && ./meta3
  circle meta3_2.cxx && ./meta3_2

circle meta4.cxx && ./meta4
  circle meta4_2.cxx && ./meta4_2

circle meta5.cxx && ./meta5
  circle meta5_2.cxx && ./meta5_2
  circle meta5_3.cxx && ./meta5_3

circle meta6.cxx && ./meta6

circle commit1.cxx && ./commit1
circle commit2.cxx && ./commit2

circle sort.cxx && ./sort

circle call.cxx && ./call

circle visit.cxx && ./visit

circle monster.cxx && ./monster
circle monster2.cxx && ./monster2