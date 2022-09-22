# Trying to open different version of Ghidra file

I believe I want to use ghidra.app.util.exporter.CppExporter

Need to pass it a DomainObject for the version of the program to export. Can be obtained like this:

getProjectRootFolder() : DomainFolder
-> getFile() : DomainFile
-> getImmutableDomainObject() : DomainObject
-> CppExporter.export()

Might want to first make a copy of each version into a new folder so that decompilation info can be cached, instead of having to regenerate it every time on export?

# Export generation

To get the real directory of the current project: DomainFolder.getProjectLocator()
TODO: how to not re-analyze the file each time we try to export it?
    Look at how Ghidra does it when you tell it to export the current program to C?