<!--
     Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

This application tests N-to-M dataport connections in a single address space.
There used to be an issue where dataports would step on each other's toes in a
single address space and only one of the dataports would end up being
considered part of the connection.
