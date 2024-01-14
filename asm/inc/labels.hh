//===================================================================================================================
// labels.hh -- functions for managing labels in the assembled output.
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of labels.  A label may be already declared (backward
//  reference) or something that is to be forward declared.  Once a label's location is determined, all references
//  to the label are cleaned up.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


