         Name: ResourcesCustom
       Status: Windows(Y), Linux(Y)
        Level: Advanced
   Maintainer: Magnus Norddahl (mbn)
      Summary: Resource manager, user-defined resources

This example is intended to provide information about 
 * user-defined resources; if you want to create your own resource types,
   and loading them using a CL_ResourceData object.

This is not such a good example showing the true use of custom resources,
custom resources using CL_ResourceData is really only useful when something
is to be shared between the objects created from the resource. Perhaps
a better example will come in a later release.

Most users will more easily understand how to use the plain resource access
through the DOM model, and probably won't need more than that; check the
Resources example for info on that.

For more information about resources take a look at the section on resources
in the overview: ClanLib/Documentation/Overview/
or on the net at http://www.clanlib.org
