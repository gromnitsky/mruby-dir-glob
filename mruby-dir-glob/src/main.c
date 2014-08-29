#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

#include "fnmatch.h"

static mrb_value
mrb_dir_mkdir(mrb_state *mrb, mrb_value klass)
{
	mrb_value m_pattern, m_string;
	mrb_int m_flags = 0;
	char *pattern, *string;
	int result;

	mrb_get_args(mrb, "SS|i", &m_pattern, &m_string, &m_flags);
	pattern = mrb_str_to_cstr(mrb, m_pattern);
	string = mrb_str_to_cstr(mrb, m_string);

	result = fnmatch(pattern, string, m_flags);

	return mrb_fixnum_value(result);
}

void
mrb_mruby_dir_glob_example_gem_init(mrb_state *mrb) {
	FileClass = mrb_define_class(mrb, "File", mrb->object_class);
	MRB_SET_INSTANCE_TT(FileClass, MRB_TT_DATA);

	mrb_define_class_method(mrb, FileClass, "fnmatch",
							mrb_fnmatch, MRB_ARGS_REQ(2)|MRB_ARGS_OPT(1));
}

void
mrb_mruby_dir_glob_example_gem_final(mrb_state *mrb) {
	/* empty */
}
