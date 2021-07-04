<!--
     Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

This application demonstrates dataport release/acquire semantics.
Dataports are regions of shared memory, and aren't marked volatile,
so the compiler is free to re-order access to them in ways that make
it impossible to use them for synchronization. The acquire/release
functions insert barriers that prevent this re-ordering.
