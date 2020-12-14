def day_cc_solution(year, day, srcs=None, data=None, deps=None, visibility=None):
    day_lib_name = '{}_lib'.format(day)
    day_check_name = '{}_check'.format(day)
    day_leaks_name = '{}_leaks'.format(day)
    day_path = '{}/{}'.format(year, day)

    native.cc_library(
        name = day_lib_name,
        srcs = srcs or ['sol.c'],
        deps = ['//day_base:day_result'] + (deps or []),
        visibility = ['//visibility:private'],
    )

    native.cc_binary(
        name = day,
        deps = ['//day_base:day_run', ':' + day_lib_name],
        data = data or ['example', 'input'],
        visibility = visibility,
    )

    native.cc_test(
        name = day_check_name,
        args = [day_path],
        deps = ['//day_base:day_check', ':' + day_lib_name],
        data = data or ['example', 'input'],
        visibility = ['//visibility:private'],
    )

    native.sh_test(
        name = day_leaks_name,
        srcs = ['//day_base:leaks-test.sh'],
        args = ['$(location //{}:{})'.format(day_path, day_check_name), day_path],
        data = [':' + day_check_name],
        visibility = ['//visibility:private'],
    )
