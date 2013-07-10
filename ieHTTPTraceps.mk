
ieHTTPTraceps.dll: dlldata.obj ieHTTPTrace_p.obj ieHTTPTrace_i.obj
	link /dll /out:ieHTTPTraceps.dll /def:ieHTTPTraceps.def /entry:DllMain dlldata.obj ieHTTPTrace_p.obj ieHTTPTrace_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ieHTTPTraceps.dll
	@del ieHTTPTraceps.lib
	@del ieHTTPTraceps.exp
	@del dlldata.obj
	@del ieHTTPTrace_p.obj
	@del ieHTTPTrace_i.obj
