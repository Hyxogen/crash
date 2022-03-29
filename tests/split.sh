printf \"%s\"\\n $PATH${IFS=: }

printf \"%s\"\\n ${CRASH-}${CRASH-}A${CRASH-}""${CRASH-}
printf \"%s\"\\n ${CRASH- }${CRASH- }A${CRASH- }""${CRASH- }
printf \"%s\"\\n "${CRASH-}""${CRASH-}"A"${CRASH-}""""${CRASH-}"
printf \"%s\"\\n "${CRASH- }""${CRASH- }"A"${CRASH- }""""${CRASH- }"
printf \"%s\"\\n " ${CRASH-} "" ${CRASH-} "A" ${CRASH-} """" ${CRASH-} "
printf \"%s\"\\n " ${CRASH- } "" ${CRASH- } "A" ${CRASH- } """" ${CRASH- } "
printf \"%s\"\\n ${CRASH-:}${CRASH-:}A${CRASH-:}""${CRASH-:}
printf \"%s\"\\n ${CRASH- : }${CRASH- : }A${CRASH- : }""${CRASH- : }
printf \"%s\"\\n "${CRASH-:}""${CRASH-:}"A"${CRASH-:}""""${CRASH-:}"
printf \"%s\"\\n "${CRASH- : }""${CRASH- : }"A"${CRASH- : }""""${CRASH- : }"
printf \"%s\"\\n " ${CRASH-:} "" ${CRASH-:} "A" ${CRASH-:} """" ${CRASH-:} "
printf \"%s\"\\n " ${CRASH- : } "" ${CRASH- : } "A" ${CRASH- : } """" ${CRASH- : } "
printf \"%s\"\\n ${CRASH-::}${CRASH-::}A${CRASH-::}""${CRASH-::}
printf \"%s\"\\n ${CRASH- : : }${CRASH- : : }A${CRASH- : : }""${CRASH- : : }
printf \"%s\"\\n "${CRASH-::}""${CRASH-::}"A"${CRASH-::}""""${CRASH-::}"
printf \"%s\"\\n "${CRASH- : : }""${CRASH- : : }"A"${CRASH- : : }""""${CRASH- : : }"
printf \"%s\"\\n " ${CRASH-::} "" ${CRASH-::} "A" ${CRASH-::} """" ${CRASH-::} "
printf \"%s\"\\n " ${CRASH- : : } "" ${CRASH- : : } "A" ${CRASH- : : } """" ${CRASH- : : } "
printf \"%s\"\\n ${CRASH-a:b}${CRASH-a:b}A${CRASH-a:b}""${CRASH-a:b}
printf \"%s\"\\n ${CRASH- a : b }${CRASH- a : b }A${CRASH- a : b }""${CRASH- a : b }
printf \"%s\"\\n "${CRASH-a:b}""${CRASH-a:b}"A"${CRASH-a:b}""""${CRASH-a:b}"
printf \"%s\"\\n "${CRASH- a : b }""${CRASH- a : b }"A"${CRASH- a : b }""""${CRASH- a : b }"
printf \"%s\"\\n " ${CRASH-a:b} "" ${CRASH-a:b} "A" ${CRASH-a:b} """" ${CRASH-a:b} "
printf \"%s\"\\n " ${CRASH- a : b } "" ${CRASH- a : b } "A" ${CRASH- a : b } """" ${CRASH- a : b } "
printf \"%s\"\\n ${CRASH-:a:b:}${CRASH-:a:b:}A${CRASH-:a:b:}""${CRASH-:a:b:}
printf \"%s\"\\n ${CRASH- : a : b : }${CRASH- : a : b : }A${CRASH- : a : b : }""${CRASH- : a : b : }
printf \"%s\"\\n "${CRASH-:a:b:}""${CRASH-:a:b:}"A"${CRASH-:a:b:}""""${CRASH-:a:b:}"
printf \"%s\"\\n "${CRASH- : a : b : }""${CRASH- : a : b : }"A"${CRASH- : a : b : }""""${CRASH- : a : b : }"
printf \"%s\"\\n " ${CRASH-:a:b:} "" ${CRASH-:a:b:} "A" ${CRASH-:a:b:} """" ${CRASH-:a:b:} "
printf \"%s\"\\n " ${CRASH- : a : b : } "" ${CRASH- : a : b : } "A" ${CRASH- : a : b : } """" ${CRASH- : a : b : } "
printf \"%s\"\\n ${CRASH-::a::b::}${CRASH-::a::b::}A${CRASH-::a::b::}""${CRASH-::a::b::}
printf \"%s\"\\n ${CRASH- : : a : : b : : }${CRASH- : : a : : b : : }A${CRASH- : : a : : b : : }""${CRASH- : : a : : b : : }
printf \"%s\"\\n "${CRASH-::a::b::}""${CRASH-::a::b::}"A"${CRASH-::a::b::}""""${CRASH-::a::b::}"
printf \"%s\"\\n "${CRASH- : : a : : b : : }""${CRASH- : : a : : b : : }"A"${CRASH- : : a : : b : : }""""${CRASH- : : a : : b : : }"
printf \"%s\"\\n " ${CRASH-::a::b::} "" ${CRASH-::a::b::} "A" ${CRASH-::a::b::} """" ${CRASH-::a::b::} "
printf \"%s\"\\n " ${CRASH- : : a : : b : : } "" ${CRASH- : : a : : b : : } "A" ${CRASH- : : a : : b : : } """" ${CRASH- : : a : : b : : } "