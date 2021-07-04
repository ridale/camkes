<!--
     Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

This application tests that the heaps of colocated component instances do not
overlap. Previously, the semantics of single address space components was that
component instances would end up sharing an enlarged heap. This has some subtle
issue and we now separate the component instances' heaps. This application
tests that these semantics are currently used.
