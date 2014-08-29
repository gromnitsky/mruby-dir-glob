#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

#include "fnmatch.h"

static mrb_value
mrb_fnmatch(mrb_state *mrb, mrb_value klass)
{
	mrb_value m_pattern, m_string;
	mrb_int m_flags = 0;
	char *pattern, *string;
	int result;

	mrb_get_args(mrb, "SS|i", &m_pattern, &m_string, &m_flags);
	pattern = mrb_str_to_cstr(mrb, m_pattern);
	string = mrb_str_to_cstr(mrb, m_string);

	result = fnmatch(pattern, string, m_flags);

	return mrb_bool_value(result == 0);
}

void
mrb_mruby_dir_glob_gem_init(mrb_state *mrb) {
	struct RClass *io = mrb_define_class(mrb, "IO", mrb->object_class);
	struct RClass *file = mrb_define_class(mrb, "File", io);

	MRB_SET_INSTANCE_TT(file, MRB_TT_DATA);

	mrb_define_class_method(mrb, file, "fnmatch",
							mrb_fnmatch, MRB_ARGS_REQ(2)|MRB_ARGS_OPT(1));
	mrb_define_class_method(mrb, file, "fnmatch?",
							mrb_fnmatch, MRB_ARGS_REQ(2)|MRB_ARGS_OPT(1));

	mrb_define_const(mrb, file, "FNM_NOESCAPE", mrb_fixnum_value(FNM_NOESCAPE));
	mrb_define_const(mrb, file, "FNM_PATHNAME", mrb_fixnum_value(FNM_PATHNAME));
	mrb_define_const(mrb, file, "FNM_DOTMATCH", mrb_fixnum_value(FNM_DOTMATCH));
	mrb_define_const(mrb, file, "FNM_CASEFOLD", mrb_fixnum_value(FNM_CASEFOLD));
}

void
mrb_mruby_dir_glob_gem_final(mrb_state *mrb) {
	/* empty */
}
