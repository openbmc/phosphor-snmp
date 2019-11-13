sdbuspp = find_program('sdbus++')

server_hpp = custom_target(
    'server.hpp',
    capture: true,
    command: [
        sdbuspp,
        '-r', meson.source_root(),
        'interface',
        'server-header',
        'xyz.openbmc_project.Network.Client.Create'
    ],
    input: '../Create.interface.yaml',
    output: 'server.hpp',
    install: true,
    install_dir: get_option('includedir') / 'xyz/openbmc_project/Network/Client/Create'
)

server_cpp = custom_target(
    'server.cpp',
    capture: true,
    command: [
        sdbuspp,
        '-r', meson.source_root(),
        'interface',
        'server-cpp',
        'xyz.openbmc_project.Network.Client.Create'
    ],
    input: '../Create.interface.yaml',
    depends : server_hpp,
    output: 'server.cpp'
)
