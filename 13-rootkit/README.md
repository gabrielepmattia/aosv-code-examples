Loadable Kernel Module Rootkit
==============================

This is a LKM which hides itself from the system. It offers several features:

- When loaded, the module starts invisible;
- Hide/unhide any process by sending a signal 31;
- Sending a signal 63(to any pid) makes the module become (in)visible;
- Sending a signal 64(to any pid) makes the given user become root;
- Files or directories starting with the MAGIC_PREFIX become invisble;

Compatibility
-------------

Works with Kernel >= 2.6


Usage
-----

Once loaded, the module is not visible in the system. To make it
visible, a 63 signal has to be sent to any process, as:

`kill -63 0`

The module has to made visible in order to let the kernel unload it.

A user can become root by sending signal 64 to any process, as:

`kill -64 0`

If you want to hide any running process from the system, hit it
with signal 31 as:

`kill -31 <pid>`


License
-------

The original code is Copyright (c) 2014, Victor N. Ramos Mello
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
