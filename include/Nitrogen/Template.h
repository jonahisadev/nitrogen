#ifndef NITROGEN_TEMPLATE_H
#define NITROGEN_TEMPLATE_H

namespace Nitrogen {

	static const char* VM_START = 
R"str(; Auto generated by Nitrogen Compiler
; Do NOT edit!

#section TEXT

start:
	; Set up machine
	call @_main
	mov ebx erx
	exit

)str";

	static const char* VM_LABEL = R"str(_%s:
)str";

	static const char* VM_RETURN_CONST = 
R"str(	mov erx %d
)str";

	static const char* VM_RETURN_G_VAR = 
R"str(	ld%s erx $g_%s
)str";

	static const char* VM_RETURN_P_VAR =
R"str(	%sget erx (ebp)+%d
)str";

	static const char* VM_VAR_DEC_AND_INIT = 
R"str(g_%s: d%s %d
)str";

	static const char* VM_VAR_SET =
R"str(	st%s $g_%s %d
)str";

}

#endif // NITROGEN_TEMPLATE_H