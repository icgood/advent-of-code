def day_cc_solution(year, day,
                    sol_file='sol.c', input_file='input', example_file='example',
                    deps=None, visibility=None):
    day_lib_name = '{}_lib'.format(day)
    day_test_name = '{}_test'.format(day)
    day_leaks_name = '{}_leaks'.format(day)
    day_path = '{}/{}'.format(year, day)

    native.cc_library(
        name = day_lib_name,
        srcs = [sol_file],
        deps = ['//day_base:day_result'] + (deps or []),
        visibility = ['//visibility:private'],
    )

    native.cc_binary(
        name = day,
        deps = [':' + day_lib_name, '//day_base:day_result_run'],
        visibility = visibility,
    )

    native.cc_test(
        name = day_test_name,
        args = [day_path],
        deps = [':' + day_lib_name, '//day_base:day_result_test'],
        data = [input_file, example_file],
        visibility = ['//visibility:private'],
    )

    native.sh_test(
        name = day_leaks_name,
        srcs = ['//day_base:leaks-test.sh'],
        args = ['$(location //{}:{})'.format(day_path, day_test_name), day_path],
        data = [':' + day_test_name],
        visibility = ['//visibility:private'],
    )
