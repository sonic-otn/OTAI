#    Copyright (c) 2015 Microsoft Open Technologies, Inc.
#	 Copyright (c) 2021 Alibaba Group.
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#

.PHONY: test doc clean

doc: meta/xml
	@echo Documentation is available at ./meta/html/

meta/xml:
	make -C meta xml

test:
	make -C test

clean:
	make -C meta clean
	make -C test clean
