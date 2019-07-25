project('NimbleEngine', 'cpp', default_options : ['cpp_std=c++17'])
add_project_link_arguments(['-lstdc++fs'], language: 'cpp')
# Renderer backend stuff
subdir('src')


# Frontend
subdir('frontend')

run_target('install-resources',
command : ['scripts/install-resources.py', './resources', './build/resources'], )

catch = subproject('catch2')

nimbleTests = executable('nimbleTests', './tests/nimble/scene-graph.cpp', dependencies: [NimbleGraphicsDepend, catch.get_variable('catch2_dep')])
test('NimbleTests', nimbleTests)