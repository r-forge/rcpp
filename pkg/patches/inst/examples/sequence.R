# -*- mode: R; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
#
# Copyright (C) 2010	Romain Francois
#
# This file is part of patches.
#
# patches is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# patches is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with patches.  If not, see <http://www.gnu.org/licenses/>.

x <- 1:10000
cat( "base::sequence : \n" )
system.time( sequence( x ) )

cat( "after loading patches :\n" )
require( patches )
system.time( sequence(x) )


