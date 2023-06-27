#!/usr/bin/perl
#
# Copyright (c) 2014 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
# @file    parse.pl
#
# @brief   This module defines OTAI Metadata Parser
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;

#use XML::Simple qw(:strict);
use Getopt::Std;
use Data::Dumper;
use utils;
use xmlutils;
use style;
use serialize;
use spec;
use cap;

our $XMLDIR = "xml";
our $INCLUDE_DIR = "../inc/";

our $OTAI_VER = "";
our %OTAI_ENUMS = ();
our %OTAI_UNIONS = ();
our %METADATA = ();
our %NON_OBJECT_ID_STRUCTS = ();
our %NOTIFICATIONS = ();
our %OBJTOAPIMAP = ();
our %APITOOBJMAP = ();
our %ALL_STRUCTS = ();
our %OBJECT_TYPE_MAP = ();
our %OTAI_DEFINES = ();
our %EXTRA_RANGE_DEFINES = ();
our %REVGRAPH = ();
our %EXTENSIONS_ENUMS = ();
our %EXTENSIONS_ATTRS = ();
our %EXPERIMENTAL_OBJECTS = ();
our %OBJECT_TYPE_TO_STATS_MAP = ();
our %OBJECT_TYPE_TO_ALARMS_MAP = ();
our %ATTR_TO_CALLBACK = ();
our %PRIMITIVE_TYPES = ();

my $FLAGS = "MANDATORY_ON_CREATE|CREATE_ONLY|CREATE_AND_SET|READ_ONLY|SET_ONLY|KEY";

# TAGS HANDLERS

my %ATTR_TAGS = (
        "unit"           , \&ProcessTagUnit,
        "precision"      , \&ProcessTagPrecision,
        "iscounter"      , \&ProcessTagIsCounter,
        "isrecoverable"  , \&ProcessTagIsRecoverable,
        "type"           , \&ProcessTagType,
        "flags"          , \&ProcessTagFlags,
        "objects"        , \&ProcessTagObjects,
        "allownull"      , \&ProcessTagAllowNull,
        "condition"      , \&ProcessTagCondition,
        "validonly"      , \&ProcessTagCondition, # since validonly uses same format as condition
        "default"        , \&ProcessTagDefault,
        "ignore"         , \&ProcessTagIgnore,
        "getsave"        , \&ProcessTagGetSave,
        "range"          , \&ProcessTagRange,
        "isresourcetype" , \&ProcessTagIsRecourceType,
        "deprecated"     , \&ProcessTagDeprecated,
        );

my %options = ();
getopts("dsASl", \%options);

our $optionPrintDebug        = 1 if defined $options{d};
our $optionDisableAspell     = 1 if defined $options{A};
our $optionUseXmlSimple      = 1 if defined $options{s};
our $optionDisableStyleCheck = 1 if defined $options{S};
our $optionShowLogCaller     = 1 if defined $options{l};

# LOGGING FUNCTIONS HELPERS

$SIG{__DIE__} = sub
{
    LogError "FATAL ERROR === MUST FIX === : @_";
    exit 1;
};

my %VALUE_TYPES = ();
my %VALUE_TYPES_TO_VT = ();

my %CAPABILITIES = ();

sub ProcessTagUnit
{
    my ($unit, $value, $val) = @_;
    return $val;
}

sub ProcessTagPrecision
{
    my ($precision, $value, $val) = @_;
    return $val;
}

sub ProcessTagIsCounter
{
    my ($type, $value, $val) = @_;
    return $val if $val =~ /^(true|false)$/i;

    LogError "iscounter tag value '$val', expected true/false";
    return undef;
}

sub ProcessTagIsRecoverable
{
    my ($type, $value, $val) = @_;
    return $val if $val =~ /^(true|false)$/i;

    LogError "isrecoverable tag value '$val', expected true/false";
    return undef;
}

sub ProcessTagType
{
    my ($type, $value, $val) = @_;

    return $val if $val =~ /^otai_s32_list_t otai_\w+_t$/;

    return $val if $val =~ /^(bool|char)$/;

    return $val if $val =~ /^otai_\w+_t$/ and not $val =~ /_attr_(extensions_)?t/;

    return $val if $val =~ /^otai_pointer_t otai_\w+_notification_fn$/;

    if ($val =~ /^otai_pointer_t (otai_linecard_\w+_fn)$/)
    {
        $ATTR_TO_CALLBACK{$value} = $1;
        return $val;
    }

    if ($val =~ /^otai_pointer_t (otai_aps_\w+_fn)$/)
    {
        $ATTR_TO_CALLBACK{$value} = $1;
        return $val;
    }

    if ($val =~ /^otai_pointer_t (otai_otdr_\w+_fn)$/)
    {
        $ATTR_TO_CALLBACK{$value} = $1;
        return $val;
    }

    if ($val =~ /^otai_pointer_t (otai_ocm_\w+_fn)$/)
    {
        $ATTR_TO_CALLBACK{$value} = $1;
        return $val;
    }

    LogError "invalid type tag value '$val' expected otai type or enum";

    return undef;
}

sub ProcessTagFlags
{
    my ($type, $value, $val) = @_;

    $val =~ s/\s*//g;

    my @flags = split/\|/,$val;

    for my $flag (@flags)
    {
        if (not $flag =~ /^($FLAGS)$/)
        {
            LogError "invalid flags tag value '$val' ($flag) on $value, expected in '$FLAGS'";
            return undef;
        }
    }

    return \@flags;
}

sub ProcessTagObjects
{
    my ($type, $value, $val) = @_;

    $val =~ s/\s*//g;

    my @ots = split/[,]/,$val;

    for my $ot (@ots)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_\w+$/)
        {
            LogError "invalid objecttype tag value '$val' ($ot) in $type $value";
            return undef;
        }
    }

    return \@ots;
}

sub ProcessTagAllowNull
{
    my ($type, $value, $val) = @_;

    return $val if $val =~ /^(true|false)$/;

    LogError "allownull tag value '$val', expected true/false";
    return undef;
}

sub ProcessTagCondition
{
    my ($type, $value, $val) = @_;

    my @conditions = split/\s+(?:or|and)\s+/,$val;

    if ($val =~ /or.+and|and.+or/)
    {
        LogError "mixed conditions and/or is not supported: $val";
        return undef;
    }

    for my $cond (@conditions)
    {
        if (not $cond =~ /^(OTAI_\w+) == (true|false|OTAI_\w+|$NUMBER_REGEX)$/)
        {
            LogError "invalid condition tag value '$val' ($cond), expected OTAI_ENUM == true|false|OTAI_ENUM|number";
            return undef;
        }
    }

    # if there is only one condition, then type does not matter

    $type = "OTAI_ATTR_CONDITION_TYPE_" . (($val =~ /and/) ? "AND" : "OR");

    unshift @conditions, $type;

    return \@conditions;
}

sub ProcessTagDefault
{
    my ($type, $value, $val) = @_;

    return $val if $val =~ /^(empty|internal|vendor|const)/;

    return $val if $val =~ /^(attrvalue) OTAI_\w+_ATTR_\w+$/;

    return $val if $val =~ /^(true|false|NULL|OTAI_\w+|$NUMBER_REGEX)$/ and not $val =~ /_ATTR_|OBJECT_TYPE/;

    return $val if $val =~ /^0\.0\.0\.0$/;

    return $val if $val eq "disabled";

    return $val if $val eq "\"\"";

    LogError "invalid default tag value '$val' on $type $value";
    return undef;
}

sub ProcessTagIgnore
{
    # just return true if tag is defined

    return "true";
}

sub ProcessTagIsRecourceType
{
    my ($type, $value, $val) = @_;

    return $val if $val =~ /^(true|false)$/i;

    LogError "isresourcetype tag value '$val', expected true/false";
    return undef;
}

sub ProcessTagDeprecated
{
    # just return true if defined

    my ($type, $value, $val) = @_;

    LogError "deprecated tag should not have value '$val'" if not $val =~ /^$/i;

    return "true";
}

sub ProcessTagRange
{
    my ($type, $attrName, $value) = @_;

    $value = Trim $value;

    if (not $value =~ /^OTAI_\w+$/)
    {
        LogWarning "invalid range value: '$value', expected 'OTAI_\\w+'";
        return "-1"
    }

    LogInfo "Creating range attrs $attrName .. MAX";

    my $range = $OTAI_DEFINES{$value};

    if (not defined $range or not $range =~ /^$NUMBER_REGEX$/)
    {
        LogWarning "range '$value' is not defined or not numer";
        return "-1";
    }

    $range = hex $range;

    if ($range < 0 or $range > 0x100)
    {
        LogWarning "range $value $range is negative or > 0x100";
        next;
    }

    return $range;
}

sub ProcessEnumItemDescription
{
    my ($type, $value, $desc) = @_;

    my @order = ();

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $val = $2;

        push @order,$tag;

        $val = Trim $val;

        next if $tag eq "ignore";

        LogError "tag '$tag' is not supported on enum ${type}::$value: $val";
    }
}

sub ProcessStatDescription
{
    my ($type, $value, $desc, $brief) = @_;

    my @order = ();

    $desc =~ s/@@/\n@@/g;
    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $val = $2;

        $val = Trim $val;

        if (not defined $ATTR_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $type $value";
            next;
        }

        $val = $ATTR_TAGS{$tag}->($type, $value, $val);

        $METADATA{$type}{$value}{$tag}          = $val;
        $METADATA{$type}{$value}{objecttype}    = $type;
        $METADATA{$type}{$value}{attrid}        = $value;
    }

    $brief = Trim $brief;

    $METADATA{$type}{$value}{brief} = $brief if $brief ne "";
}

sub ProcessDescription
{
    my ($type, $value, $desc, $brief) = @_;

    my @order = ();

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $val = $2;

        push @order,$tag;

        $val = Trim $val;

        if (not defined $ATTR_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $type $value";
            next;
        }

        $val = $ATTR_TAGS{$tag}->($type, $value, $val);

        $METADATA{$type}{$value}{$tag}          = $val;
        $METADATA{$type}{$value}{objecttype}    = $type;
        $METADATA{$type}{$value}{attrid}        = $value;
    }

    $brief = Trim $brief;

    $METADATA{$type}{$value}{brief} = $brief if $brief ne "";

    return if scalar@order == 0;

    my $rightOrder = 'type:flags(:isrecoverable)?(:objects)?(:allownull)?(:default)?(:range)?(:condition|:validonly)?(:isresourcetype)?(:deprecated)?';

    my $order = join(":",@order);

    return if $order =~ /^$rightOrder$/;
    return if $order =~ /^ignore$/;

    LogWarning "metadata tags are not in right order: $order on $value";
    LogWarning "   correct order: $rightOrder or ignore";
}

sub ProcessDefineSection
{
    my $section = shift;

    for my $memberdef (@{ $section->{memberdef} })
    {
        next if not $memberdef->{kind} eq "define";

        my $name = $memberdef->{name}[0];

        next if (not $name =~ /^OTAI_\w+$/);

        my $initializer = $memberdef->{initializer}[0];

        next if (not $initializer =~ /^(\(?".*"|$NUMBER_REGEX\)?)$/);

        $OTAI_DEFINES{$name} = $1;

        LogDebug "adding define $name = $initializer";
    }
}

sub ProcessEnumSection
{
    my $section = shift;

    for my $memberdef (@{ $section->{memberdef} })
    {
        next if not $memberdef->{kind} eq "enum";

        my $enumtypename = $memberdef->{name}[0];

        $enumtypename =~ s/^_//;

        if (not $enumtypename =~ /^(otai_\w+_)t$/)
        {
            LogWarning "enum $enumtypename is not prefixed otai_";
            next;
        }

        my $enumprefix = uc $1;

        if ($enumtypename =~ /_extensions_t$/)
        {
            LogDebug "removing extension prefix from $enumtypename";

            # remove extensions suffix on all extensions since they will be merged together
            $enumprefix =~ s/EXTENSIONS_$//;

            $EXTENSIONS_ENUMS{$enumtypename} = "${enumprefix}_t";
        }

        if (defined $OTAI_ENUMS{$enumtypename})
        {
            LogError "duplicated enum $enumtypename";
            next;
        }

        my $ed = ExtractDescription($enumtypename, $enumtypename, $memberdef->{detaileddescription}[0]);

        $OTAI_ENUMS{$enumtypename}{flagsenum} = ($ed =~ /\@\@flags/s) ? "true" : "false";

        my @arr = ();

        $OTAI_ENUMS{$enumtypename}{values} = \@arr;

        for my $ev (@{ $memberdef->{enumvalue} })
        {
            my $enumvaluename = $ev->{name}[0];

            my $eitemd = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);

            if ($eitemd =~ /\@ignore/)
            {
                LogInfo "Ignoring $enumvaluename";

                # process ignore attributes

                if (not defined $OTAI_ENUMS{$enumtypename}{ignoreval})
                {
                    my @ignoreval = ();
                    $OTAI_ENUMS{$enumtypename}{ignoreval} = \@ignoreval;
                }

                my $ref = $OTAI_ENUMS{$enumtypename}{ignoreval};
                push @$ref, $enumvaluename;

                next;
            }

            LogDebug "$enumtypename $enumvaluename";

            push@arr,$enumvaluename;

            LogWarning "Value $enumvaluename of $enumtypename is not prefixed as $enumprefix" if not $enumvaluename =~ /^$enumprefix/;

            if (not $enumvaluename =~ /^[A-Z0-9_]+$/)
            {
                LogError "enum $enumvaluename uses characters outside [A-Z0-9_]+";
            }
        }

        # remove unnecessary attributes
        my @values = @{ $OTAI_ENUMS{$enumtypename}{values} };

        @values = grep(!/^OTAI_\w+_(ATTR|STAT|RANGE)_(START|END)$/, @values);
        @values = grep(!/^OTAI_\w+(RANGE_BASE)$/, @values);

        if ($enumtypename =~ /^(otai_\w+)_t$/)
        {
            my $valuescount = @values;

            # allow empty enum on extensions
            if ($valuescount == 0 and not $enumtypename =~ /_extensions_t$/)
            {
                LogError "enum $enumtypename is empty, after removing suffixed entries _START/_END/_RANGE_BASE";
                LogError "  those suffixes are reserved for range markers and are removed by metadata parser, don't use them";
                LogError "  as actual part of valid enum name, take a look at otai_udf_group_type_t for valid usage";
                next;
            }

            if ($valuescount > 0)
            {
                my $last = $values[$#values];

                if ($last eq "${enumprefix}MAX")
                {
                    $last =  pop @values;
                    LogInfo "Removing last element $last";
                }
            }
        }

        $OTAI_ENUMS{$enumtypename}{values} = \@values;

        if ($enumtypename =~ /^(otai_(\w+)_stat_)t$/)
        {
            my $prefix = uc$1;
            for my $ev (@{ $memberdef->{enumvalue} })
            {
                my $enumvaluename = $ev->{name}[0];
                my $desc = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);
                my $brief = ExtractDescription($enumtypename, $enumvaluename, $ev->{briefdescription}[0]);
                ProcessStatDescription($enumtypename, $enumvaluename, $desc, $brief);
            }
            next;
        }


        if (not $enumtypename =~ /^(otai_(\w+)_attr_(extensions_)?)t$/)
        {
            for my $ev (@{ $memberdef->{enumvalue} })
            {
                my $enumvaluename = $ev->{name}[0];

                my $eitemd = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);

                ProcessEnumItemDescription($enumtypename, $enumvaluename, $eitemd);
            }

            next;
        }

        # ENUM ATTRIBUTES PROCESSED BELOW

        # TODO put to OTAI_ATTR_ENUMS

        my $prefix = uc$1;

        # remove unnecessary attributes
        @values = @{ $OTAI_ENUMS{$enumtypename}{values} };
        @values = grep(!/^${prefix}(CUSTOM_RANGE_|FIELD_|ACTION_)?(START|END)$/, @values);

        $OTAI_ENUMS{$enumtypename}{values} = \@values;

        # this is attribute

        for my $ev (@{ $memberdef->{enumvalue} })
        {
            my $enumvaluename = $ev->{name}[0];

            my $desc = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);
            my $brief = ExtractDescription($enumtypename, $enumvaluename, $ev->{briefdescription}[0]);

            ProcessDescription($enumtypename, $enumvaluename, $desc, $brief);

            # remove ignored attributes from enums from list
            # since enum must match attribute list size

            if (defined $METADATA{$enumtypename}{$enumvaluename}{ignore})
            {
                @values = grep(!/^$enumvaluename$/, @values);
                $OTAI_ENUMS{$enumtypename}{values} = \@values;
                next;
            }

            if ($enumvaluename =~ /^(OTAI_\w+_)MIN$/)
            {
                my $prefix = $1;

                my $range = $METADATA{$enumtypename}{$enumvaluename}{range};

                if (not defined $range)
                {
                    # XXX we can relax this and generate range only if range tag is defined
                    LogWarning "attribute $enumvaluename must have \@range tag";
                    next;
                }

                my $rangeLimit = 10;

                if ($range > $rangeLimit)
                {
                    # let's not generate too many attributes that will not be used

                    LogInfo "Limiting range from $range to $rangeLimit";

                    $range = $rangeLimit;
                }

                # we assume zero is reserved for *_MIN and last value for *_MAX

                my @rangeElements = ();

                for (my $idx = 1; $idx < $range; ++$idx)
                {
                    my $attrid = "${prefix}$idx";

                    push@rangeElements, $attrid;

                    my $attr = ShallowCopyAttrEnum($METADATA{$enumtypename}{$enumvaluename});

                    $attr->{attrid} = $attrid;

                    $METADATA{$enumtypename}{$attrid} = $attr;

                    $EXTRA_RANGE_DEFINES{$attrid} = "($enumvaluename + $idx)";
                }

                # update enum values (this could be done in previous step

                my @values = @{ $OTAI_ENUMS{$enumtypename}{values} };

                my ($index) = grep { $values[$_] =~ /^$enumvaluename$/ } 0..$#values;

                splice @values, $index+1,0, @rangeElements;

                $OTAI_ENUMS{$enumtypename}{values} = \@values;
            }
        }
    }
}

sub ShallowCopyAttrEnum
{
    my $refHash = shift;

    my %hash = %{ $refHash };

    my %attr = map { $_, $hash{$_} } keys %hash;

    return \%attr;
}

sub ProcessPrimitiveTypedef
{
    my ($typedeftype, $definition) = @_;

    if (not $definition =~ /^typedef (u?int\d+_t) ((otai_\w+_t)(\[\d+\])?)$/)
    {
        LogError("unrecognized primitive type: '$definition'");
        return;
    }

    my $base = $1;
    my $fulltype = $2;
    my $name = $3;

    $PRIMITIVE_TYPES{$name}{base} = $base;
    $PRIMITIVE_TYPES{$name}{fulltype} = $fulltype;
    $PRIMITIVE_TYPES{$name}{isarray} = ( $fulltype =~ /\[\d+\]/ ) ? 1 : 0;
}

sub ProcessTypedefSection
{
    my $section = shift;

    for my $memberdef (@{ $section->{memberdef} })
    {
        next if not $memberdef->{kind} eq "typedef";

        my $id = $memberdef->{id};

        my $typedefname = $memberdef->{name}[0];

        my $typedeftype;

        $typedeftype = $memberdef->{type}[0] if ref $memberdef->{type}[0] eq "";

        $typedeftype = $memberdef->{type}[0]->{content} if ref $memberdef->{type}[0] eq "HASH";

        ProcessPrimitiveTypedef($typedeftype, $memberdef->{definition}[0]) if $typedeftype =~ /^u?int\d+_t$/;

        if ($typedeftype =~ /^struct/)
        {
            # record structs for later serialization
            # this will also include structs from metadata
            $ALL_STRUCTS{$typedefname} = 1;
            next;
        }

        if ($typedefname =~ /^otai_\w+_notification_fn$/)
        {
            if (not $typedeftype =~ /void\(\*/)
            {
                LogWarning "notification $typedefname should return void, but got '$typedeftype'";
                next;
            }

            ProcessNotifications($memberdef, $typedefname);
            next;
        }

        # TODO add callback handling

        next if not $typedeftype =~ /^enum/;

        if (not defined $OTAI_ENUMS{$typedefname})
        {
            LogError "enum $typedefname has no typedef enum $typedefname";
            next;
        }

        next if not $typedefname =~ /^otai_(\w+)_attr_(extensions_)?t$/;

        # this enum is attribute definition for object

        $OTAI_ENUMS{$typedefname}{objecttype} = "OTAI_OBJECT_TYPE_" . uc($1);
    }
}

sub ProcessNotificationCount
{
    my ($ntfName, $tagValue, $previousTagValue) = @_;

    my %count = ();

    %count = %{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(\w+)\[(\w+|\d+)\]$/g)
    {
        LogError "unable to parse count '$tagValue' on $ntfName";
        return undef;
    }

    my $pointerParam = $1;
    my $countParam = $2;

    $count{$pointerParam} = $countParam;

    LogDebug "adding count $pointerParam\[$countParam\] on $ntfName";

    if ($pointerParam eq $countParam)
    {
        LogError "count '$pointerParam' can't point to itself in \@count on $ntfName";
        undef;
    }

    return \%count;
}

sub ProcessNotificationObjects
{
    #
    # object type for attribute params are described in
    # notification descripnon, it would be easier
    # if they would be described on params itself
    #

    my ($ntfName, $tagValue, $previousTagValue) = @_;

    my %objectTypes = ();

    %objectTypes = %{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(\w+)\s+(OTAI_OBJECT_TYPE_\w+)$/g)
    {
        LogError "invalid object type tag value '$tagValue' in $ntfName";
        return undef;
    }

    $objectTypes{$1} = $2;

    LogDebug "adding object type $2 on param $1 in $ntfName";

    return \%objectTypes;
}

my %NOTIFICATION_TAGS = (
        "count"       , \&ProcessNotificationCount,
        "objects"     , \&ProcessNotificationObjects,
        );

sub ProcessNotificationDescription
{
    my ($refNtf, $desc) = @_;

    $refNtf->{desc} = $desc;

    my $ntfName = $refNtf->{name};

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $value = Trim($2);

        if (not defined $NOTIFICATION_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $ntfName: $value";
            next;
        }

        LogDebug "processing tag '$tag' on $ntfName";

        $refNtf->{$tag} = $NOTIFICATION_TAGS{$tag}->($ntfName, $value, $refNtf->{$tag});
    }
}

sub ProcessNotifications
{
    my ($member, $typedefname) = @_;

    my $args = $member->{argsstring}[0];

    $args =~ s/[()]//g;

    my @params = split/,/,$args;

    my %N = (name => $typedefname);

    my $desc = ExtractDescription($typedefname, $typedefname, $member->{detaileddescription}[0]);

    ProcessNotificationDescription(\%N, $desc);

    my @Members = ();

    my $idx = 0;

    my $ParamRegex = '^\s*_In_ ((const )?\w+\s*?\*?)\s*(\w+)$';

    my @keys = ();

    for my $param (@params)
    {
        # NOTE multple pointers or consts are not supported

        if (not $param =~ /$ParamRegex/)
        {
            LogWarning "can't match param '$param' on $typedefname";
            return
        }

        my %M = ();

        my $type = $1;
        my $name = $3;

        push @keys, $name;

        if (defined $N{objects} and defined $N{objects}{$name})
        {
            my @objects = ();

            push @objects, $N{objects}{$name};

            $M{objects} = \@objects;
        }

        $M{param}       = $param;
        $M{type}        = $type;
        $M{name}        = $name;
        $M{idx}         = $idx;
        $M{file}        = $member->{location}[0]->{file};

        push @Members, \%M;

        $N{membersHash}{ $name } = \%M;

        $idx++;
    }

    # second pass is needed if count param is defined after data pointer

    for my $param (@params)
    {
        next if (not $param =~ /$ParamRegex/);

        my $type = $1;
        my $name = $3;

        next if not $type =~ /\*/;

        if (not defined $N{count} and not defined $N{count}->{$name})
        {
            LogWarning "type '$type' is pointer, \@count is required, but missing on $typedefname";
            next;
        }

        my $countParamName = $N{count}->{$name};

        $N{membersHash}->{$name}{count} = $countParamName;

        if ($countParamName =~ /^(\d+)$/)
        {
            # count is given explicit
            next;
        }

        if (not defined $N{membersHash}->{$countParamName})
        {
            LogWarning "count param name '$countParamName' on $typedefname is not defined";
            next;
        }

        my $countType = $N{membersHash}->{$countParamName}{type};

        if (not $countType =~ /^(uint32_t|otai_size_t)$/)
        {
            LogWarning "param '$countParamName' used as count for param '$name' ($typedefname)";
            LogWarning " is wrong type '$countType' allowed: (uint32_t|otai_size_t)";
        }
    }

    $N{params} = \@params;
    $N{keys} = \@keys;
    $N{members} = \@Members;
    $N{ismethod} = 1;

    $N{baseName} = $1 if $typedefname =~ /^otai_(\w+_notification)_fn$/;

    $NOTIFICATIONS{$typedefname} = \%N;
}

sub ProcessXmlFile
{
    my $file = shift;

    my $ref = ReadXml $file;

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    for my $section (@sections)
    {
        ProcessDefineSection($section) if ($section->{kind} eq "define");

        ProcessEnumSection($section) if ($section->{kind} eq "enum");

        ProcessTypedefSection($section) if $section->{kind} eq "typedef";

    }
    if ($file =~ /^xml\/otai_8h\.xml$/)
    {
        my $bd = ${$ref->{compounddef}[0]->{briefdescription}}[0];
        if (defined $bd->{para})
        {
            my @para = @{ $bd->{para} };
            for my $p (@para)
            {
               next if not $p =~ /^This module defines an entry point into OTAI-(\d.\d+).$/;
               $OTAI_VER = $1;
            }
        }
    }
}

sub ProcessSingleEnum
{
    my ($key, $typedef, $prefix) = @_;

    $prefix =~ s/EXTENSIONS_$// if ($typedef =~ /_extensions_t$/);

    my $enum = $OTAI_ENUMS{$key};

    my @values = @{$enum->{values}};

    my $flags = (defined $enum->{flagsenum}) ? $enum->{flagsenum} : "false";

    WriteSource "const $typedef otai_metadata_${typedef}_enum_values[] = {";

    for my $value (@values)
    {
        LogWarning "Value $value of $typedef must use only capital letters" if $value =~ /[a-z]/;

        LogWarning "Value $value of $typedef is not prefixed as $prefix" if not $value =~ /^$prefix/;

        WriteSource "$value,";
    }

    WriteSource "-1"; # guard
    WriteSource "};";

    WriteSource "const char* const otai_metadata_${typedef}_enum_values_names[] = {";

    for my $value (@values)
    {
        WriteSource "\"$value\",";
    }

    WriteSource "NULL";
    WriteSource "};";

    WriteSource "const char* const otai_metadata_${typedef}_enum_values_short_names[] = {";

    for my $value (@values)
    {
        $value =~ s/^${prefix}//;

        WriteSource "\"$value\",";
    }

    WriteSource "NULL";
    WriteSource "};";

    if (defined $enum->{ignoreval})
    {
        my @ignoreval = @{ $enum->{ignoreval} };

        WriteSource "const $typedef otai_metadata_${typedef}_enum_ignore_values[] = {";

        for my $value (@ignoreval)
        {
            WriteSource "$value,";
        }

        WriteSource "-1"; # guard
        WriteSource "};";

        WriteSource "const char* const otai_metadata_${typedef}_enum_ignore_values_names[] = {";

        for my $value (@ignoreval)
        {
            WriteSource "\"$value\",";
        }

        WriteSource "NULL";
        WriteSource "};";
    }

    my $count = @values;

    WriteHeader "extern const otai_enum_metadata_t otai_metadata_enum_$typedef;";

    WriteSource "const otai_enum_metadata_t otai_metadata_enum_$typedef = {";
    WriteSource ".name              = \"${typedef}\",";
    WriteSource ".valuescount       = $count,";
    WriteSource ".values            = (const int*)otai_metadata_${typedef}_enum_values,";
    WriteSource ".valuesnames       = otai_metadata_${typedef}_enum_values_names,";
    WriteSource ".valuesshortnames  = otai_metadata_${typedef}_enum_values_short_names,";
    WriteSource ".containsflags     = $flags,";

    if (defined $enum->{ignoreval})
    {
        WriteSource ".ignorevalues      = (const int*)otai_metadata_${typedef}_enum_ignore_values,";
        WriteSource ".ignorevaluesnames = otai_metadata_${typedef}_enum_ignore_values_names,";
    }
    else
    {
        WriteSource ".ignorevalues      = NULL,";
        WriteSource ".ignorevaluesnames = NULL,";
    }

    WriteSource "};";

    return $count;
}

sub ProcessExtraRangeDefines
{
    WriteSectionComment "Enums metadata";

    for my $key (sort keys %EXTRA_RANGE_DEFINES)
    {
        WriteHeader "#define $key $EXTRA_RANGE_DEFINES{$key}";
    }
}

sub CreateMetadataHeaderAndSource
{
    WriteSource "#include <stdio.h>";
    WriteSource "#include <string.h>";
    WriteSource "#include <stdlib.h>";
    WriteSource "#include <stddef.h>";
    WriteSource "#include \"otaimetadata.h\"";

    WriteSectionComment "Enums metadata";

    for my $key (sort keys %OTAI_ENUMS)
    {
        if (not $key =~ /^((otai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching OTAI format";
            next;
        }

        ProcessSingleEnum($key, $1, uc $2);
    }

    # all enums

    WriteHeader "extern const otai_enum_metadata_t* const otai_metadata_all_enums[];";
    WriteSource "const otai_enum_metadata_t* const otai_metadata_all_enums[] = {";

    for my $key (sort keys %OTAI_ENUMS)
    {
        if (not $key =~ /^((otai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching OTAI format";
            next;
        }

        my $typedef = $1;

        WriteSource "&otai_metadata_enum_$typedef,";
    }

    WriteSource "NULL";
    WriteSource "};";

    my $count = keys %OTAI_ENUMS;

    WriteHeader "extern const size_t otai_metadata_all_enums_count;";
    WriteSource "const size_t otai_metadata_all_enums_count = $count;";

    WriteHeader "extern const otai_enum_metadata_t* const otai_metadata_attr_enums[];";
    WriteSource "const otai_enum_metadata_t* const otai_metadata_attr_enums[] = {";

    $count = 0;

    for my $key (sort keys %OTAI_ENUMS)
    {
        next if not $key =~ /^(otai_\w+_attr_t)$/;

        my $typedef = $1;

        WriteSource "&otai_metadata_enum_$typedef,";

        $count++;
    }

    WriteSource "NULL";
    WriteSource "};";

    WriteHeader "extern const size_t otai_metadata_attr_enums_count;";
    WriteSource "const size_t otai_metadata_attr_enums_count = $count;";

    # attr enums as object types for sanity check

    WriteSource "const otai_object_type_t otai_metadata_object_types[] = {";

    for my $key (sort keys %OTAI_ENUMS)
    {
        if (not $key =~ /^(otai_(\w+)_attr_t)$/)
        {
            next;
        }

        my $typedef = $1;
        my $objtype = $2;

        WriteSource "OTAI_OBJECT_TYPE_" . uc($objtype). ",";
    }

    WriteSource "-1";
    WriteSource "};";
}

sub ProcessStatUnit
{
    my ($stat, $unit) = @_;
    if (not defined $unit)
    {
        return "OTAI_STAT_VALUE_UNIT_NORMAL";
    }
    return "OTAI_STAT_VALUE_UNIT_DBM" if $unit eq "dBm";
    return "OTAI_STAT_VALUE_UNIT_DB" if $unit eq "dB";

    return "OTAI_STAT_VALUE_UNIT_NORMAL";
}

sub ProcessStatPrecision
{
    my ($stat, $precision) = @_;
    if (not defined $precision)
    {
        return "OTAI_STAT_VALUE_PRECISION_0";
    }
    return "OTAI_STAT_VALUE_PRECISION_1" if $precision eq "precision1";
    return "OTAI_STAT_VALUE_PRECISION_2" if $precision eq "precision2";
    return "OTAI_STAT_VALUE_PRECISION_18" if $precision eq "precision18";

    return "OTAI_STAT_VALUE_PRECISION_0";
}

sub ProcessStatIsCounter
{
    my ($stat, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessStatType
{
    my ($stat, $type) = @_;

    if (not defined $type)
    {
        LogError "type is not defined for $stat";
        return "";
    }

    if ($type =~ /^(otai_\w+_t)$/)
    {
        my $prefix = "OTAI_STAT_VALUE_TYPE";

        return "OTAI_STAT_VALUE_TYPE_DOUBLE" if $1 eq "otai_double_t";
        return "${prefix}_$VALUE_TYPES_TO_VT{$1}" if defined $VALUE_TYPES_TO_VT{$1};

        if (not defined $OTAI_ENUMS{$1})
        {
            LogError "invalid enum specified '$type' on $stat";
            return "";
        }

        return "${prefix}_INT32";
    }

    LogError "invalid type '$type' for $stat";
    return "";
}

sub ProcessStatName
{
    my ($stat, $type) = @_;

    return "\"$stat\"";
}

sub ProcessStatKebabName
{
    my ($stat, $type) = @_;
    my $kebab;

    if ($stat =~ /^(OTAI_\w+_STAT_)(\w+)$/) {
        $kebab = lc $2;
        $kebab =~ s/_/-/g;
    }

    return "\"$kebab\"";
}

sub ProcessStatCamelName
{
    my ($stat, $type) = @_;
    my $camel;

    if ($stat =~ /^(OTAI_\w+_STAT_)(\w+)$/) {
        $camel = lc $2;
        $camel =~ s/(_|^)(.)/\u$2/g;
    }

    return "\"$camel\"";
}

sub ProcessType
{
    my ($attr, $type) = @_;

    if (not defined $type)
    {
        LogError "type is not defined for $attr";
        return "";
    }

    if ($type =~ /^otai_s32_list_t (otai_\w+_t)$/)
    {
        if (not defined $OTAI_ENUMS{$1})
        {
            LogError "invalid enum list specified '$type' on $attr";
            return "";
        }

        return "OTAI_ATTR_VALUE_TYPE_INT32_LIST";
    }

    if ($type eq "otai_double_t")
    {
        return "OTAI_ATTR_VALUE_TYPE_DOUBLE";
    }

    if ($type =~ /^(otai_\w+_t)$/)
    {
        my $prefix = "OTAI_ATTR_VALUE_TYPE";

        return "${prefix}_$VALUE_TYPES_TO_VT{$1}" if defined $VALUE_TYPES_TO_VT{$1};

        if (not defined $OTAI_ENUMS{$1})
        {
            LogError "invalid enum specified '$type' on $attr";
            return "";
        }

        return "${prefix}_INT32";
    }

    if ($type eq "bool")
    {
        return "OTAI_ATTR_VALUE_TYPE_BOOL";
    }

    if ($type eq "char")
    {
        return "OTAI_ATTR_VALUE_TYPE_CHARDATA";
    }

    if ($type =~ /^otai_pointer_t otai_\w+_fn$/)
    {
        return "OTAI_ATTR_VALUE_TYPE_POINTER";
    }

    LogError "invalid type '$type' for $attr";
    return "";
}

sub ProcessFlags
{
    my ($value,$flags) = @_;

    if (not defined $flags)
    {
        LogError "flags are not defined for $value";
        return "";
    }

    my @flags = @{ $flags };

    @flags = map {s/^/OTAI_ATTR_FLAGS_/; $_; } @flags;

    return "(otai_attr_flags_t)(" . join("|",@flags) . ")";
}

sub ProcessAllowNull
{
    my ($value,$allownull) = @_;

    return $allownull if defined $allownull;

    return "false";
}

sub ProcessIsRecoverable
{
    my ($value,$isrecoverable) = @_;

    return $isrecoverable if defined $isrecoverable;

    return "true";
}

sub ProcessIsResourceType
{
    my ($value,$isresourcetype) = @_;

    return $isresourcetype if defined $isresourcetype;

    return "false";
}

sub ProcessIsDeprecatedType
{
    my ($value, $deprecated) = @_;

    return $deprecated if defined $deprecated;

    return "false";
}

sub ProcessObjects
{
    my ($attr, $objects) = @_;

    return "NULL" if not defined $objects;

    WriteSource "const otai_object_type_t otai_metadata_${attr}_allowed_objects[] = {";

    my @all = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $obj (@{ $objects })
    {
        if (not defined $OBJECT_TYPE_MAP{$obj})
        {
            LogError "unknown object type '$obj' on $attr";
            return "";
        }

        WriteSource "$obj,";
    }

    WriteSource "-1"; # guard
    WriteSource "};";

    return "otai_metadata_${attr}_allowed_objects";
}

sub ProcessObjectsLen
{
    my ($value, $objects) = @_;

    return "0" if not defined $objects;

    my $count = @{ $objects };

    return $count;
}

sub ProcessDefaultValueType
{
    my ($attr, $default) = @_;

    return "OTAI_DEFAULT_VALUE_TYPE_NONE" if not defined $default;

    return "OTAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^OTAI_NULL_OBJECT_ID$/;

    return "OTAI_DEFAULT_VALUE_TYPE_LINECARD_INTERNAL" if $default =~ /^internal$/;

    return "OTAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^(true|false|const|NULL|$NUMBER_REGEX|OTAI_\w+)$/ and not $default =~ /_ATTR_|OTAI_OBJECT_TYPE_/;

    return "OTAI_DEFAULT_VALUE_TYPE_EMPTY_LIST" if $default =~ /^empty$/;

    return "OTAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC" if $default =~ /^vendor$/;

    return "OTAI_DEFAULT_VALUE_TYPE_ATTR_VALUE" if $default =~ /^attrvalue OTAI_\w+$/ and $default =~ /_ATTR_/;

    return "OTAI_DEFAULT_VALUE_TYPE_ATTR_RANGE" if $default =~ /^attrrange OTAI_\w+$/ and $default =~ /_ATTR_/;

    return "OTAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^0\.0\.0\.0$/;

    return "OTAI_DEFAULT_VALUE_TYPE_CONST" if $default eq "disabled";

    return "OTAI_DEFAULT_VALUE_TYPE_CONST" if $default eq "\"\"";

    LogError "invalid default value type '$default' on $attr";

    return "";
}

sub ProcessDefaultValue
{
    my ($attr, $default, $type) = @_;

    return "NULL" if not defined $default;

    my $val = "const otai_attribute_value_t otai_metadata_${attr}_default_value";

    if ($default =~ /^(true|false)$/ and $type eq "bool")
    {
        WriteSource "$val = { .booldata = $default };";
    }
    elsif ($default =~ /^OTAI_NULL_OBJECT_ID$/ and $type =~ /^otai_object_id_t$/)
    {
        WriteSource "$val = { .oid = $default };";
    }
    elsif ($default =~ /^OTAI_\w+$/ and $type =~ /^otai_\w+_t$/ and not defined $VALUE_TYPES{$type})
    {
        WriteSource "$val = { .s32 = $default };";
    }
    elsif ($default =~ /^$NUMBER_REGEX$/ and $type =~ /^otai_u?int\d+_t/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$type} = $default };";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /^(otai_pointer_t) (otai_\w+_fn)$/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$1} = $default };";
    }
    elsif ($default =~ /^(attrvalue|attrrange|vendor|empty|const|internal)/)
    {
        return "NULL";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /^otai_pointer_t/)
    {
        LogError "missing typedef function in format 'otai_\\w+_fn' on $attr ($type)";
    }
    elsif ($default =~ /^0\.0\.0\.0$/ and $type =~ /^(otai_ip_address_t)/)
    {
        # ipv4 address needs to be converted to uint32 number so we support now only 0.0.0.0

        WriteSource "$val = { .$VALUE_TYPES{$1} = { .addr_family = OTAI_IP_ADDR_FAMILY_IPV4, .addr = { .ip4 = 0 } } };";
    }
    elsif ($default =~ /^""$/ and $type eq "char")
    {
        WriteSource "$val = { .chardata = { 0 } };";
    }
    elsif ($default =~ /^0\.0\.0\.0$/ and $type =~ /^(otai_ip4_t)/)
    {
        WriteSource "$val = { 0 };";
    }
    else
    {
        LogError "invalid default value '$default' on $attr ($type)";
    }

    return "&otai_metadata_${attr}_default_value";
}

sub ProcessDefaultValueObjectType
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return "OTAI_OBJECT_TYPE_$2" if $value =~ /^(attrvalue|attrrange) OTAI_(\w+)_ATTR_\w+$/;

    return "OTAI_OBJECT_TYPE_NULL";
}

sub ProcessDefaultValueAttrId
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return $2 if $value =~ /^(attrvalue|attrrange) ((OTAI_\w+)_ATTR_\w+)$/;

    return "OTAI_INVALID_ATTRIBUTE_ID";
}

sub ProcessStoreDefaultValue
{
    my ($attr, $value, $flags) = @_;

    # at this point we can't determine whether object is non object id

    if (defined $value and $value =~ /vendor|attrvalue/)
    {
        return "true";
    }

    my @flags = @{ $flags };

    $flags = "@flags";

    if ($flags =~ /MANDATORY/ and $flags =~ /CREATE_AND_SET/)
    {
        return "true";
    }

    return "false";
}

sub ProcessIsEnum
{
    my ($value, $type) = @_;

    return "false" if not defined $type;

    return "true" if $type =~ /^otai_\w+_t$/ and not defined $VALUE_TYPES{$type};

    return "false";
}

sub ProcessIsEnumList
{
    my ($attr, $type) = @_;

    return "false" if not defined $type;

    return "true" if $type =~ /^otai_s32_list_t otai_\w+_t$/;

    return "false";
}

sub ProcessEnumMetadata
{
    my ($attr, $type) = @_;

    return "NULL" if not defined $type;

    return "&otai_metadata_enum_$1" if $type =~ /^(otai_\w+_t)$/ and not defined $VALUE_TYPES{$type};
    return "&otai_metadata_enum_$1" if $type =~ /^otai_s32_list_t (otai_\w+_t)$/;

    return "NULL";
}

sub ProcessGetSave
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessConditionType
{
    my ($attr, $value) = @_;

    return "OTAI_ATTR_CONDITION_TYPE_NONE" if not defined $value;

    return @{$value}[0];
}

sub ProcessConditionsGeneric
{
    my ($attr, $conditions, $enumtype, $name) = @_;

    return "NULL" if not defined $conditions;

    my @conditions = @{ $conditions };

    shift @conditions;

    my $count = 0;

    my @values = ();

    for my $cond (@conditions)
    {
        if (not $cond =~ /^(OTAI_\w+) == (true|false|OTAI_\w+|$NUMBER_REGEX)$/)
        {
            LogError "invalid condition '$cond' on $attr";
            return "";
        }

        my $attrid = $1;
        my $val = $2;

        my $main_attr = $1 if $attr =~ /^OTAI_(\w+?)_ATTR_/;
        my $cond_attr = $1 if $attrid =~ /^OTAI_(\w+?)_ATTR_/;

        if ($main_attr ne $cond_attr)
        {
            LogError "$name attribute $attr has condition from different object $attrid";
            return "";
        }

        WriteSource "const otai_attr_condition_t otai_metadata_${name}_${attr}_$count = {";

        if ($val eq "true" or $val eq "false")
        {
            WriteSource ".attrid = $attrid,";
            WriteSource ".condition = { .booldata = $val }";
        }
        elsif ($val =~ /^OTAI_/)
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .s32 = $val }";

            my $attrType = lc("$1t") if $attrid =~ /^(OTAI_\w+_ATTR_)/;

            my $enumTypeName = $METADATA{$attrType}{$attrid}{type};

            if (not defined $enumTypeName)
            {
                LogError("failed to find attribute ${attrType}::${attrid} when processing $attrid");
                next;
            }

            if (defined $OTAI_ENUMS{$enumTypeName})
            {
                # this condition is enum condition, check if condition value
                # belongs to that enum

                my @values = @{ $OTAI_ENUMS{$enumTypeName}{values} };

                if (not grep( /^$val$/, @values))
                {
                    LogError "condition value '$val' in '$cond' on $attr is not present in $enumTypeName";
                }
            }
        }
        elsif ($val =~ /^$NUMBER_REGEX$/ and $enumtype =~ /^otai_u?int(\d+)_t$/)
        {
            my $n = $1;
            my $item = ($enumtype =~ /uint/) ? "u$n" : "s$n";

            WriteSource ".attrid = $attrid,";
            WriteSource ".condition = { .$item = $val }";
        }
        else
        {
            LogError "unknown $name value: $val on $attr $enumtype";
            return "";
        }

        WriteSource "};";

        $count++;
    }

    WriteSource "const otai_attr_condition_t* const otai_metadata_${name}s_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "&otai_metadata_${name}_${attr}_$count,";

        $count++;
    }

    WriteSource "NULL";
    WriteSource "};";

    return "otai_metadata_${name}s_${attr}";
}

sub ProcessConditions
{
    ProcessConditionsGeneric(@_, "condition");
}

sub ProcessConditionsLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    # NOTE: number returned is -1 since first item is condition type

    return $#conditions;
}

sub ProcessValidOnlyType
{
    my ($attr, $value) = @_;

    return "OTAI_ATTR_CONDITION_TYPE_NONE" if not defined $value;

    return @{$value}[0];
}

sub ProcessValidOnly
{
    ProcessConditionsGeneric(@_, "validonly");
}

sub ProcessValidOnlyLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    # NOTE: number returned is -1 since first item is condition type

    return $#conditions;
}

sub ProcessAllowRepeat
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAllowMixed
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAllowEmpty
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAttrName
{
    my ($attr, $type) = @_;

    return "\"$attr\"";
}

sub ProcessAttrKebabName
{
    my ($attr, $type) = @_;
    my $kebabname;

    if ($attr =~ /^(OTAI_\w+_ATTR_)(\w+)$/) {
        $kebabname = lc $2;
        $kebabname =~ s/_/-/g;
    }

    return "\"$kebabname\"";
}

sub ProcessIsCallback
{
    my ($attr, $type) = @_;

    return "true" if defined $ATTR_TO_CALLBACK{$attr};

    return "false";
}

sub ProcessNotificationType
{
    my ($attr, $type) = @_;

    return "OTAI_LINECARD_NOTIFICATION_TYPE_$1" if $attr =~ /^OTAI_LINECARD_ATTR_(\w+)_NOTIFY$/;

    return "-1";
}

sub ProcessBrief
{
    my ($attr, $brief) = @_;

    if (not defined $brief or $brief eq "")
    {
        LogWarning "missing brief description for $attr";
        return "";
    }

    if ($brief =~ m!([^\x20-\x7e]|\\|")!is)
    {
        LogWarning "Not allowed char '$1' in brief description on $attr: $brief";
    }

    if (length $brief > 200)
    {
        LogWarning "Long brief > 200 on $attr:\n - $brief";
    }

    return "\"$brief\"";
}

sub ProcessIsPrimitive
{
    my ($attr, $type) = @_;

    return "false" if $type =~ /(_list_t)/;

    return "true";
}

sub ProcessCapability
{
    my ($attr, $type, $enummetadata) = @_;

    return "NULL" if not defined $CAPABILITIES{$attr};

    my %CAP = %{ $CAPABILITIES{$attr} };

    my $count = 0;

    my @values = ();

    for my $vid (sort keys %CAP)
    {
        my $enumcount = 0;
        my $enumvalues = "NULL";

        if (defined $CAP{$vid}{enumcapability})
        {
            if (not $enummetadata =~ /otai_metadata_enum_((otai_\w+_)t)/)
            {
                LogError "enum capability defined on $attr, but attribute is not enum";
                next;
            }

            my $enumtype = $1;
            my $prefix = uc($2);

            my @values = @{ $CAP{$vid}{enumcapability} };

            $enumcount = scalar @values;

            WriteSource "const int otai_metadata_enumcapability_${attr}_$vid\[\] = {";

            my %vals = ();

            for my $v (@values)
            {
                LogError "enumvalue $v on capability $attr($vid) is not of type $enumtype" if not $v =~ /^$prefix/;

                LogError "enumvalue $v on capability $attr($vid) is already defined" if defined $vals{$v};

                $vals{$v} = 1;

                WriteSource "   $v,";
            }

            WriteSource "   -1,";
            WriteSource "};";

            $enumvalues = "otai_metadata_enumcapability_${attr}_$vid";
        }

        if (not defined $CAP{$vid}{capability})
        {
            LogError "capability for $attr is not defined";
            next;
        }

        WriteSource "const otai_attr_capability_metadata_t otai_metadata_attr_capability_${attr}_$count = {";

        my %cap = ();

        for my $c (@{ $CAP{$vid}{capability} })
        {
            $cap{$c} = 1;
        }

        my $create = (defined $cap{"CREATE"}) ? "true" : "false";
        my $get = (defined $cap{"GET"}) ? "true" : "false";
        my $set = (defined $cap{"SET"}) ? "true" : "false";

        WriteSource "    .vendorid = $vid,";
        WriteSource "    .operationcapability = {";
        WriteSource "       .create_implemented = $create,";
        WriteSource "       .set_implemented = $set,";
        WriteSource "       .get_implemented = $get,";
        WriteSource "    },";

        WriteSource "    .enumvaluescount = $enumcount,";
        WriteSource "    .enumvalues = $enumvalues,";

        WriteSource "};";

        $count++;
    }

    WriteSource "const otai_attr_capability_metadata_t* const otai_metadata_attr_capability_${attr}\[\] = {";

    $count = 0;

    for my $vid (sort keys %CAP)
    {
        WriteSource "    &otai_metadata_attr_capability_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "otai_metadata_attr_capability_$attr";
}

sub ProcessCapabilityLen
{
    my ($attr, $type) = @_;

    return 0 if not defined $CAPABILITIES{$attr};

    return scalar(keys %{$CAPABILITIES{$attr}});
}

sub ProcessIsExtensionAttr
{
    my ($attr, $type) = @_;

    return "true" if defined $EXTENSIONS_ATTRS{$attr};

    return "false";
}

sub ProcessSingleObjectTypeStat
{
    my ($typedef, $objecttype) = @_;

    my $enum = $OTAI_ENUMS{$typedef};

    my @values = @{ $enum->{values} };

    for my $stat (@values)
    {
        if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$stat})
        {
            LogError "metadata is missing for $stat";
            next;
        }

        my %meta = %{ $METADATA{$typedef}{$stat} };

        $meta{type} = "" if not defined $meta{type};

        my $type            = ProcessStatType($stat, $meta{type});
        my $statname        = ProcessStatName($stat, $meta{type});
        my $unit            = ProcessStatUnit($stat, $meta{unit});
        my $precision       = ProcessStatPrecision($stat, $meta{precision});
        my $iscounter       = ProcessStatIsCounter($stat, $meta{iscounter});
        my $kebabname       = ProcessStatKebabName($stat, $meta{type});
        my $camelname       = ProcessStatCamelName($stat, $meta{type});

        WriteSource "const otai_stat_metadata_t otai_metadata_stat_$stat = {";

        WriteSource ".objecttype                    = $objecttype,";
        WriteSource ".statid                        = $stat,";
        WriteSource ".statidname                    = $statname,";
        WriteSource ".statvaluetype                 = $type,";
        WriteSource ".statvalueunit                 = $unit,";
        WriteSource ".statvalueprecision            = $precision,";
        WriteSource ".statvalueiscounter            = $iscounter,";
        WriteSource ".statidkebabname               = $kebabname,";
        WriteSource ".statidcamelname               = $camelname,";
        WriteSource "};";
    }
}

sub ProcessSingleObjectType
{
    my ($typedef, $objecttype) = @_;

    my $enum = $OTAI_ENUMS{$typedef};

    my @values = @{ $enum->{values} };

    for my $attr (@values)
    {
        if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$attr})
        {
            LogError "metadata is missing for $attr";
            next;
        }

        my %meta = %{ $METADATA{$typedef}{$attr} };

        next if defined $meta{ignore};

        $meta{type} = "" if not defined $meta{type};

        my $type            = ProcessType($attr, $meta{type});
        my $attrname        = ProcessAttrName($attr, $meta{type});
        my $flags           = ProcessFlags($attr, $meta{flags});
        my $isrecoverable   = ProcessIsRecoverable($attr, $meta{isrecoverable});
        my $allownull       = ProcessAllowNull($attr, $meta{allownull});
        my $objects         = ProcessObjects($attr, $meta{objects});
        my $objectslen      = ProcessObjectsLen($attr, $meta{objects});
        my $allowrepeat     = ProcessAllowRepeat($attr, $meta{allowrepeat});
        my $allowmixed      = ProcessAllowMixed($attr, $meta{allowmixed});
        my $allowempty      = ProcessAllowEmpty($attr, $meta{allowempty});
        my $defvaltype      = ProcessDefaultValueType($attr, $meta{default});
        my $defval          = ProcessDefaultValue($attr, $meta{default}, $meta{type});
        my $defvalot        = ProcessDefaultValueObjectType($attr, $meta{default}, $meta{type});
        my $defvalattrid    = ProcessDefaultValueAttrId($attr, $meta{default}, $meta{type});
        my $storedefaultval = ProcessStoreDefaultValue($attr, $meta{default}, $meta{flags});
        my $isenum          = ProcessIsEnum($attr, $meta{type});
        my $isenumlist      = ProcessIsEnumList($attr, $meta{type});
        my $enummetadata    = ProcessEnumMetadata($attr, $meta{type});
        my $conditiontype   = ProcessConditionType($attr, $meta{condition});
        my $conditions      = ProcessConditions($attr, $meta{condition}, $meta{type});
        my $conditionslen   = ProcessConditionsLen($attr, $meta{condition});
        my $validonlytype   = ProcessValidOnlyType($attr, $meta{validonly});
        my $validonly       = ProcessValidOnly($attr, $meta{validonly}, $meta{type});
        my $validonlylen    = ProcessValidOnlyLen($attr, $meta{validonly});
        my $getsave         = ProcessGetSave($attr, $meta{getsave});
        my $brief           = ProcessBrief($attr, $meta{brief});
        my $isprimitive     = ProcessIsPrimitive($attr, $meta{type});
        my $ntftype         = ProcessNotificationType($attr, $meta{type});
        my $iscallback      = ProcessIsCallback($attr, $meta{type});
        my $cap             = ProcessCapability($attr, $meta{type}, $enummetadata);
        my $caplen          = ProcessCapabilityLen($attr, $meta{type});
        my $isextensionattr = ProcessIsExtensionAttr($attr, $meta{type});
        my $isresourcetype  = ProcessIsResourceType($attr, $meta{isresourcetype});
        my $isdeprecated    = ProcessIsDeprecatedType($attr, $meta{deprecated});

        my $ismandatoryoncreate = ($flags =~ /MANDATORY/)       ? "true" : "false";
        my $iscreateonly        = ($flags =~ /CREATE_ONLY/)     ? "true" : "false";
        my $iscreateandset      = ($flags =~ /CREATE_AND_SET/)  ? "true" : "false";
        my $isreadonly          = ($flags =~ /READ_ONLY/)       ? "true" : "false";
        my $issetonly           = ($flags =~ /SET_ONLY/)        ? "true" : "false";
        my $iskey               = ($flags =~ /KEY/)             ? "true" : "false";

        my $kebabname           = ProcessAttrKebabName($attr, $meta{type});

        WriteSource "const otai_attr_metadata_t otai_metadata_attr_$attr = {";

        WriteSource ".objecttype                    = $objecttype,";
        WriteSource ".attrid                        = $attr,";
        WriteSource ".attridname                    = $attrname,";
        WriteSource ".brief                         = $brief,";
        WriteSource ".attrvaluetype                 = $type,";
        WriteSource ".flags                         = $flags,";
        WriteSource ".isrecoverable                 = $isrecoverable,";
        WriteSource ".allowedobjecttypes            = $objects,";
        WriteSource ".allowedobjecttypeslength      = $objectslen,";
        WriteSource ".allowrepetitiononlist         = $allowrepeat,";
        WriteSource ".allowmixedobjecttypes         = $allowmixed,";
        WriteSource ".allowemptylist                = $allowempty,";
        WriteSource ".allownullobjectid             = $allownull,";
        WriteSource ".isoidattribute                = ($objectslen > 0),";
        WriteSource ".defaultvaluetype              = $defvaltype,";
        WriteSource ".defaultvalue                  = $defval,";
        WriteSource ".defaultvalueobjecttype        = $defvalot,";
        WriteSource ".defaultvalueattrid            = $defvalattrid,";
        WriteSource ".storedefaultvalue             = $storedefaultval,";
        WriteSource ".isenum                        = $isenum,";
        WriteSource ".isenumlist                    = $isenumlist,";
        WriteSource ".enummetadata                  = $enummetadata,";
        WriteSource ".conditiontype                 = $conditiontype,";
        WriteSource ".conditions                    = $conditions,";
        WriteSource ".conditionslength              = $conditionslen,";
        WriteSource ".isconditional                 = ($conditionslen != 0),";
        WriteSource ".validonlytype                 = $validonlytype,";
        WriteSource ".validonly                     = $validonly,";
        WriteSource ".validonlylength               = $validonlylen,";
        WriteSource ".isvalidonly                   = ($validonlylen != 0),";
        WriteSource ".getsave                       = $getsave,";
        WriteSource ".ismandatoryoncreate           = $ismandatoryoncreate,";
        WriteSource ".iscreateonly                  = $iscreateonly,";
        WriteSource ".iscreateandset                = $iscreateandset,";
        WriteSource ".isreadonly                    = $isreadonly,";
        WriteSource ".issetonly                     = $issetonly,";
        WriteSource ".iskey                         = $iskey,";
        WriteSource ".isprimitive                   = $isprimitive,";
        WriteSource ".notificationtype              = $ntftype,";
        WriteSource ".iscallback                    = $iscallback,";
        WriteSource ".capability                    = $cap,";
        WriteSource ".capabilitylength              = $caplen,";
        WriteSource ".isextensionattr               = $isextensionattr,";
        WriteSource ".isresourcetype                = $isresourcetype,";
        WriteSource ".isdeprecated                  = $isdeprecated,";
        WriteSource ".attridkebabname               = $kebabname,";

        WriteSource "};";

        # check enum attributes if their names are ending on enum name

        CheckEnumNaming($attr, $meta{type}) if $isenum eq "true" or $isenumlist eq "true";
    }
}

sub CheckEnumNaming
{
    my ($attr, $type) = @_;

    LogError "can't match otai type on '$type'" if not $type =~ /.*otai_(\w+)_t/;

    my $enumTypeName = uc($1);

    return if $attr =~ /_${enumTypeName}_LIST$/;
    return if $attr =~ /_$enumTypeName$/;

    $attr =~ /OTAI_(\w+?)_ATTR(_\w+?)(_LIST)?$/;

    my $attrObjectType = $1;
    my $attrSuffix = $2;

    if ($enumTypeName =~ /^${attrObjectType}_(\w+)$/)
    {
        my $enumTypeNameSuffix = $1;

        return if $attrSuffix =~ /_$enumTypeNameSuffix$/;

        return if $attr eq "OTAI_TRANSCEIVER_ATTR_ETHERNET_PMD_PRECONF";
        return if $attr eq "OTAI_ETHERNET_ATTR_CLIENT_RX_ALS";

        LogError "attr = $attr, type = $type";
        LogError "enum starts by object type $attrObjectType but not ending on $enumTypeNameSuffix in $enumTypeName";
    }

    LogError "$type == $attr not ending on enum name $enumTypeName";
}

sub CreateMetadata
{
    for my $key (sort keys %OTAI_ENUMS)
    {
        if ($key =~ /^(otai_(\w+)_attr_t)$/)
        {
            my $typedef = $1;
            my $objtype = "OTAI_OBJECT_TYPE_" . uc($2);

            ProcessSingleObjectType($typedef, $objtype);
        }
        elsif ($key =~ /^(otai_(\w+)_stat_t)$/)
        {
            my $typedef = $1;
            my $objtype = "OTAI_OBJECT_TYPE_" . uc($2);

            ProcessSingleObjectTypeStat($typedef, $objtype);
        }
    }
}

sub ProcessLaiStatus
{
    my $filename = "../inc/otaistatus.h";

    open(my $fh, '<', $filename) or die "Could not open file '$filename' $!";

    my @values = ();

    WriteSectionComment "Extra OTAI statuses";

    while (my $line = <$fh>)
    {
        next if not $line =~ /define\s+(OTAI_STATUS_\w+).+(0x00\w+)/;

        my $status = $1;
        my $base = $2;

        push@values,$status;

        next if not ($status =~ /(OTAI_\w+)_0$/);

        for my $idx (1..10)
        {
            $status = "$1_$idx";

            WriteHeader "#define $status  OTAI_STATUS_CODE(($base + ${idx}))";

            push@values,$status;
        }
    }

    close $fh;

    $OTAI_ENUMS{"otai_status_t"}{values} = \@values;
    $OTAI_ENUMS{"otai_status_t"}{flagsenum} = "true";
}

sub CreateMetadataForStatistics
{
    my @objects = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $ot (@objects)
    {

        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "otai_" . lc($1) . "_stat_t";

        if (not defined $OTAI_ENUMS{$type})
        {
            my @empty = ();

            $OTAI_ENUMS{$type}{values} = \@empty;
        }

        WriteSource "const otai_stat_metadata_t* const otai_metadata_stat_object_type_$type\[\] = {";

        my @values = @{ $OTAI_ENUMS{$type}{values} };

        for my $value (@values)
        {
            next if defined $METADATA{$type}{$value}{ignore};

            WriteSource "&otai_metadata_stat_$value,";
        }

        WriteSource "NULL";
        WriteSource "};";
    }

    WriteHeader "extern const otai_stat_metadata_t* const* const otai_metadata_stat_by_object_type[];";
    WriteSource "const otai_stat_metadata_t* const* const otai_metadata_stat_by_object_type[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "otai_" . lc($1) . "_stat_t";

        WriteSource "otai_metadata_stat_object_type_$type,";
    }

    WriteSource "NULL";
    WriteSource "};";

    my $count = @objects;

    WriteHeader "extern const size_t otai_metadata_stat_by_object_type_count;";
    WriteSource "const size_t otai_metadata_stat_by_object_type_count = $count;";
}

sub CreateMetadataForAttributes
{
    my @objects = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $ot (@objects)
    {

        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "otai_" . lc($1) . "_attr_t";

        if (not defined $OTAI_ENUMS{$type})
        {
            my @empty = ();

            $OTAI_ENUMS{$type}{values} = \@empty;
        }

        WriteSource "const otai_attr_metadata_t* const otai_metadata_object_type_$type\[\] = {";

        my @values = @{ $OTAI_ENUMS{$type}{values} };

        for my $value (@values)
        {
            next if defined $METADATA{$type}{$value}{ignore};

            WriteSource "&otai_metadata_attr_$value,";
        }

        WriteSource "NULL";
        WriteSource "};";
    }

    WriteHeader "extern const otai_attr_metadata_t* const* const otai_metadata_attr_by_object_type[];";
    WriteSource "const otai_attr_metadata_t* const* const otai_metadata_attr_by_object_type[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "otai_" . lc($1) . "_attr_t";

        WriteSource "otai_metadata_object_type_$type,";
    }

    WriteSource "NULL";
    WriteSource "};";

    my $count = @objects;

    WriteHeader "extern const size_t otai_metadata_attr_by_object_type_count;";
    WriteSource "const size_t otai_metadata_attr_by_object_type_count = $count;";

    WriteSectionComment "Define OTAI_OBJECT_TYPE_EXTENSIONS_MAX";

    WriteHeader "#define OTAI_OBJECT_TYPE_EXTENSIONS_MAX ((otai_object_type_t)$count)";
}

sub CreateEnumHelperMethod
{
    my $key = shift;

    return if not $key =~ /^otai_(\w+)_t/;

    WriteSource "const char* otai_metadata_get_$1_name(";
    WriteSource "_In_ $key value)";
    WriteSource "{";
    WriteSource "return otai_metadata_get_enum_value_name(&otai_metadata_enum_$key, value);";
    WriteSource "}";

    WriteHeader "extern const char* otai_metadata_get_$1_name(";
    WriteHeader "_In_ $key value);\n";
}

sub CreateEnumHelperMethods
{
    WriteSectionComment "Get enum name helper methods";

    for my $key (sort keys %OTAI_ENUMS)
    {
        next if $key =~ /_attr_(extensions_)?t$/;

        next if $key =~ /_stat_t$/;

        CreateEnumHelperMethod($key);
    }
}

sub ProcessIsNonObjectId
{
    my $struct = shift;

    return "false" if not defined $struct;

    return "true";
}

sub ProcessStructValueType
{
    my $type = shift;

    return "OTAI_ATTR_VALUE_TYPE_OBJECT_ID"      if $type eq "otai_object_id_t";
    return "OTAI_ATTR_VALUE_TYPE_UINT32"         if $type eq "otai_label_id_t";
    return "OTAI_ATTR_VALUE_TYPE_UINT32"         if $type eq "uint32_t";
    return "OTAI_ATTR_VALUE_TYPE_INT32"          if $type =~ /^otai_\w+_type_t$/; # enum
    return "OTAI_ATTR_VALUE_TYPE_BOOL"           if $type eq "bool";
    return "OTAI_ATTR_VALUE_TYPE_INT32"          if defined $OTAI_ENUMS{$type}; # enum

    return "-1"                                 if $type eq "otai_attribute_t*";

    LogError "invalid struct member value type $type";

    return -1;
}

sub ProcessStructIsVlan
{
    my $type = shift;

    return "true" if $type eq "otai_vlan_id_t";

    return "false";
}

sub ProcessStructObjects
{
    my ($rawname, $key, $struct) = @_;

    my $type = $struct->{type};

    return "NULL" if not $type eq "otai_object_id_t" and not $type eq "otai_attribute_t*";

    WriteSource "const otai_object_type_t otai_metadata_struct_member_otai_${rawname}_t_${key}_allowed_objects[] = {";

    my $objects = $struct->{objects};

    for my $obj (@{ $objects })
    {
        WriteSource "$obj,";
    }

    WriteSource "-1,";

    WriteSource "};";

    return "otai_metadata_struct_member_otai_${rawname}_t_${key}_allowed_objects";
}

sub ProcessStructObjectLen
{
    my ($rawname, $key, $struct) = @_;

    my $type = $struct->{type};

    return 0 if not $type eq "otai_object_id_t" and not $type eq "otai_attribute_t*";

    my @objects = @{ $struct->{objects} };

    my $count = @objects;

    return $count;
}

sub ProcessStructEnumData
{
    my $type = shift;

    return "&otai_metadata_enum_$type" if defined $OTAI_ENUMS{$type};
    return "&otai_metadata_enum_$type" if $type =~ /^otai_\w+_type_t$/; # enum

    return "NULL";
}

sub ProcessStructIsEnum
{
    my $type = shift;

    return "true" if defined $OTAI_ENUMS{$type};
    return "true" if $type =~ /^otai_\w+_type_t$/; # enum

    return "false";
}

sub ProcessStructGetOid
{
    my ($type, $key, $rawname, $any) = @_;

    return "NULL" if $type ne "otai_object_id_t";

    my $fname = "otai_metadata_struct_member_get_otai_${rawname}_t_${key}";

    return "NULL" if (defined $any);

    WriteSource "otai_object_id_t $fname(";
    WriteSource "_In_ const otai_object_meta_key_t *object_meta_key)";
    WriteSource "{";
    WriteSource "return object_meta_key->objectkey.key.${rawname}.${key};";
    WriteSource "}";

    return $fname;
}

sub ProcessStructSetOid
{
    my ($type, $key, $rawname, $any) = @_;

    return "NULL" if $type ne "otai_object_id_t";

    my $fname = "otai_metadata_struct_member_set_otai_${rawname}_t_${key}";

    return "NULL" if (defined $any);

    WriteSource "void $fname(";
    WriteSource "_Inout_ otai_object_meta_key_t *object_meta_key,";
    WriteSource "_In_ otai_object_id_t oid)";
    WriteSource "{";
    WriteSource "object_meta_key->objectkey.key.${rawname}.${key} = oid;";
    WriteSource "}";

    return $fname;
}

sub ProcessStructOffset
{
    my ($type, $key, $rawname) = @_;

    return "offsetof(otai_${rawname}_t,$key)";
}

sub ProcessStructSize
{
    my ($type, $key, $rawname) = @_;

    return "sizeof($type)";
}

sub ProcessStructMembers
{
    my ($struct, $ot, $rawname, $any) = @_;

    return "NULL" if not defined $struct;

    my @keys = GetStructKeysInOrder($struct);

    if ($keys[0] ne "linecard_id" and not defined $any)
    {
        LogError "linecard_id is not first item in $rawname";
    }

    for my $key (@keys)
    {
        my $valuetype   = ProcessStructValueType($struct->{$key}{type});
        my $objects     = ProcessStructObjects($rawname, $key, $struct->{$key});
        my $objectlen   = ProcessStructObjectLen($rawname, $key, $struct->{$key});
        my $isenum      = ProcessStructIsEnum($struct->{$key}{type});
        my $enumdata    = ProcessStructEnumData($struct->{$key}{type});
        my $getoid      = ProcessStructGetOid($struct->{$key}{type}, $key, $rawname, $any);
        my $setoid      = ProcessStructSetOid($struct->{$key}{type}, $key, $rawname, $any);
        my $offset      = ProcessStructOffset($struct->{$key}{type}, $key, $rawname);
        my $size        = ProcessStructSize($struct->{$key}{type}, $key, $rawname);

        WriteSource "const otai_struct_member_info_t otai_metadata_struct_member_otai_${rawname}_t_$key = {";

        WriteSource ".membervaluetype           = $valuetype,";
        WriteSource ".membername                = \"$key\",";
        WriteSource ".allowedobjecttypes        = $objects,";
        WriteSource ".allowedobjecttypeslength  = $objectlen,";
        WriteSource ".isenum                    = $isenum,";
        WriteSource ".enummetadata              = $enumdata,";
        WriteSource ".getoid                    = $getoid,";
        WriteSource ".setoid                    = $setoid,";
        WriteSource ".offset                    = $offset,";
        WriteSource ".size                      = $size,";

        # TODO allow null

        WriteSource "};";

        if ($objectlen > 0 and not $key =~ /_id$/ and not defined $any)
        {
            LogWarning "struct member key '$key' should end at _id in otai_${rawname}_t";
        }
    }

    WriteSource "const otai_struct_member_info_t* const otai_metadata_struct_members_otai_${rawname}_t[] = {";

    for my $key (@keys)
    {
        WriteSource "&otai_metadata_struct_member_otai_${rawname}_t_$key,";
    }

    WriteSource "NULL";
    WriteSource "};";

    return "otai_metadata_struct_members_otai_${rawname}_t";
}

sub ProcessStructMembersCount
{
    my $struct = shift;

    return "0" if not defined $struct;

    my $count = keys %$struct;

    return $count;
}

sub ProcessRevGraph
{
    #
    # Purpose of this method is to generate metadata where current object type
    # is used since currentrly if we have attribute metadata we can easly scan
    # attributes with oids values and extract information of object being used
    # on that attribute, scanning all attributes of that object type we have
    # dependency graph
    #
    # but what we create here is reverse dependency graph it will tell us on
    # which object and which attrubute current object type is used
    #
    # we can of course create both graphs right at the same time
    #

    my %REVGRAPH = GetReverseDependencyGraph();

    my $objectType = shift;

    if (not defined $REVGRAPH{$objectType})
    {
        # some objects are not used, so they will be not defined
        return "NULL";
    }

    my @dep = @{ $REVGRAPH{$objectType} };

    @dep = sort @dep;

    my $index = 0;

    my @membernames = ();;

    for my $dep (@dep)
    {
        my ($depObjectType, $attrId) = split/,/,$dep;

        my $membername = "otai_metadata_${objectType}_rev_graph_member_$index";

        push@membernames,$membername;

        WriteSource "const otai_rev_graph_member_t $membername = {";

        WriteSource ".objecttype          = $objectType,";
        WriteSource ".depobjecttype       = $depObjectType,";

        if ($attrId =~ /^OTAI_\w+_ATTR_\w+/)
        {
            # this is attribute

            WriteSource ".attrmetadata        = &otai_metadata_attr_$attrId,";
            WriteSource ".structmember        = NULL,";
        }
        else
        {
            # this is struct member inside non object id struct

            my $DEPOT = lc ($1) if $depObjectType =~ /OTAI_OBJECT_TYPE_(\w+)/;

            WriteSource ".attrmetadata        = NULL,";
            WriteSource ".structmember        = &otai_metadata_struct_member_otai_${DEPOT}_t_$attrId,";
        }

        WriteSource "};";

        $index++;
    }

    WriteSource "const otai_rev_graph_member_t* const otai_metadata_${objectType}_rev_graph_members[] = {";

    for my $mn (@membernames)
    {
        WriteSource "&$mn,";
    }

    WriteSource "NULL";

    WriteSource "};";

    return "otai_metadata_${objectType}_rev_graph_members";
}

sub ProcessRevGraphCount
{
    my $objectType = shift;

    return 0 if not defined $REVGRAPH{$objectType};

    return scalar @{ $REVGRAPH{$objectType} };
}

sub CreateStructNonObjectId
{
    my @objects = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        next if $1 eq "NULL" or $1 eq "MAX";

        my $type = "otai_" . lc($1) . "_attr_t";

        my $enum  = "&otai_metadata_enum_${type}";

        my $struct = $NON_OBJECT_ID_STRUCTS{$ot};

        my $structmembers = ProcessStructMembers($struct, $ot ,lc($1));
    }
}

sub ProcessStructMembersName
{
    my ($struct, $ot, $rawname) = @_;

    return "NULL" if not defined $struct;

    return "otai_metadata_struct_members_otai_${rawname}_t";
}

sub ProcessCreate
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_create_$ot(";
    WriteSource "_Inout_ otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ otai_object_id_t linecard_id,";
    WriteSource "_In_ uint32_t attr_count,";
    WriteSource "_In_ const otai_attribute_t *attr_list)";
    WriteSource "{";

    if (not defined $struct)
    {
        if ($small eq "linecard")
        {
            WriteSource "return otai_metadata_otai_${api}_api->create_$small(&meta_key->objectkey.key.object_id, attr_count, attr_list);";
        }
        else
        {
            WriteSource "return otai_metadata_otai_${api}_api->create_$small(&meta_key->objectkey.key.object_id, linecard_id, attr_count, attr_list);";
        }
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->create_$small(&meta_key->objectkey.key.$small, attr_count, attr_list);";
    }

    WriteSource "}";

    return "otai_metadata_generic_create_$ot";
}

sub ProcessRemove
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_remove_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key)";
    WriteSource "{";

    if (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->remove_$small(meta_key->objectkey.key.object_id);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->remove_$small(&meta_key->objectkey.key.$small);";
    }

    WriteSource "}";

    return "otai_metadata_generic_remove_$ot";
}

sub ProcessSet
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_set_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ const otai_attribute_t *attr)";
    WriteSource "{";

    if (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->set_${small}_attribute(meta_key->objectkey.key.object_id, attr);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->set_${small}_attribute(&meta_key->objectkey.key.$small, attr);";
    }

    WriteSource "}";

    return "otai_metadata_generic_set_$ot";
}

sub ProcessGet
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_get_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ uint32_t attr_count,";
    WriteSource "_Inout_ otai_attribute_t *attr_list)";
    WriteSource "{";

    if (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_attribute(meta_key->objectkey.key.object_id, attr_count, attr_list);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_attribute(&meta_key->objectkey.key.$small, attr_count, attr_list);";
    }

    WriteSource "}";

    return "otai_metadata_generic_get_$ot";
}

sub ProcessGetStats
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_get_stats_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ uint32_t number_of_counters,";
    WriteSource "_In_ const otai_stat_id_t *counter_ids,";
    WriteSource "_Out_ otai_stat_value_t *counters)";
    WriteSource "{";

    if (not defined $OBJECT_TYPE_TO_STATS_MAP{$small})
    {
        WriteSource "return OTAI_STATUS_NOT_SUPPORTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_stats(meta_key->objectkey.key.object_id, number_of_counters, counter_ids, counters);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_stats(&meta_key->objectkey.key.$small, number_of_counters, counter_ids, counters);";
    }

    WriteSource "}";

    return "otai_metadata_generic_get_stats_$ot";
}

sub ProcessGetStatsExt
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_get_stats_ext_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ uint32_t number_of_counters,";
    WriteSource "_In_ const otai_stat_id_t *counter_ids,";
    WriteSource "_In_ otai_stats_mode_t mode,";
    WriteSource "_Out_ otai_stat_value_t *counters)";
    WriteSource "{";

    if (not defined $OBJECT_TYPE_TO_STATS_MAP{$small})
    {
        WriteSource "return OTAI_STATUS_NOT_SUPPORTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_stats_ext(meta_key->objectkey.key.object_id, number_of_counters, counter_ids, mode, counters);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->get_${small}_stats_ext(&meta_key->objectkey.key.$small, number_of_counters, counter_ids, mode, counters);";
    }

    WriteSource "}";

    return "otai_metadata_generic_get_stats_ext_$ot";
}

sub ProcessClearStats
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /OTAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "otai_status_t otai_metadata_generic_clear_stats_$ot(";
    WriteSource "_In_ const otai_object_meta_key_t *meta_key,";
    WriteSource "_In_ uint32_t number_of_counters,";
    WriteSource "_In_ const otai_stat_id_t *counter_ids)";
    WriteSource "{";

    if (not defined $OBJECT_TYPE_TO_STATS_MAP{$small})
    {
        WriteSource "return OTAI_STATUS_NOT_SUPPORTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "return otai_metadata_otai_${api}_api->clear_${small}_stats(meta_key->objectkey.key.object_id, number_of_counters, counter_ids);";
    }
    else
    {
        WriteSource "return otai_metadata_otai_${api}_api->clear_${small}_stats(&meta_key->objectkey.key.$small, number_of_counters, counter_ids);";
    }

    WriteSource "}";

    return "otai_metadata_generic_clear_stats_$ot";
}

sub CreateApis
{
    WriteSectionComment "Global OTAI API declarations";

    for my $key (sort keys %APITOOBJMAP)
    {
        WriteSource "otai_${key}_api_t *otai_metadata_otai_${key}_api = NULL;";
        WriteHeader "extern otai_${key}_api_t *otai_metadata_otai_${key}_api;";
    }
}

sub CreateApisStruct
{
    my @apis = @{ $OTAI_ENUMS{otai_api_t}{values} };

    WriteSectionComment "All APIs struct";

    WriteHeader "typedef struct _otai_apis_t {";

    for my $api (@apis)
    {
        $api =~ /^OTAI_API_(\w+)/;

        $api = lc($1);

        next if $api =~ /unspecified/;

        WriteHeader "otai_${api}_api_t* ${api}_api;";
    }

    WriteHeader "} otai_apis_t;";

    my $count = scalar @apis;

    WriteSectionComment "Define OTAI_API_EXTENSIONS_MAX";

    WriteHeader "#define OTAI_API_EXTENSIONS_MAX ((otai_api_t)$count)";
}

sub CreateGlobalApis
{
    WriteSectionComment "Global OTAI API declarations";

    WriteSource "otai_apis_t otai_metadata_apis = { 0 };";
    WriteHeader "extern otai_apis_t otai_metadata_apis;";
}

sub CreateApisQuery
{
    WriteSectionComment "OTAI API query";

    WriteHeader "typedef otai_status_t (*otai_api_query_fn)(";
    WriteHeader "_In_ otai_api_t otai_api_id,";
    WriteHeader "_Out_ void** api_method_table);";

    # for linecard we need to generate wrapper, for others we can use pointers
    # so we don't need to use meta key then

    WriteSource "int otai_metadata_apis_query(";
    WriteSource "_In_ const otai_api_query_fn api_query,";
    WriteSource "_Inout_ otai_apis_t *apis)";
    WriteSource "{";
    WriteSource "otai_status_t status = OTAI_STATUS_SUCCESS;";
    WriteSource "int count = 0;";

    WriteSource "if (api_query == NULL)";
    WriteSource "{";

    for my $key (sort keys %APITOOBJMAP)
    {
        WriteSource "otai_metadata_otai_${key}_api = NULL;";
    }

    WriteSource "memset(apis, 0, sizeof(otai_apis_t));";
    WriteSource "memset(&otai_metadata_apis, 0, sizeof(otai_apis_t));";

    WriteSource "return count;";
    WriteSource "}";

    for my $key (sort keys %APITOOBJMAP)
    {
        my $api = uc("OTAI_API_${key}");

        WriteSource "status = api_query($api, (void**)&otai_metadata_otai_${key}_api);";
        WriteSource "apis->${key}_api = otai_metadata_otai_${key}_api;";
        WriteSource "if (status != OTAI_STATUS_SUCCESS)";
        WriteSource "{";
        WriteSource "count++;";
        WriteSource "const char *name = otai_metadata_get_enum_value_name(&otai_metadata_enum_otai_status_t, status);";
        WriteSource "OTAI_META_LOG_NOTICE(\"failed to query api $api: %s (%d)\", name, status);";
        WriteSource "}";
    }

    WriteSource "return count; /* number of unsuccesfull apis */";

    WriteSource "}";

    WriteHeader "extern int otai_metadata_apis_query(";
    WriteHeader "_In_ const otai_api_query_fn api_query,";
    WriteHeader "_Inout_ otai_apis_t *apis);";
}

sub ProcessIsExperimental
{
    my $ot = shift;

    return "true" if defined $EXPERIMENTAL_OBJECTS{$ot};

    return "false";
}

sub ProcessStatEnum
{
    my $shortot = shift;

    my $statenumname = "otai_${shortot}_stat_t";

    return "&otai_metadata_enum_$statenumname" if defined $OTAI_ENUMS{$statenumname};

    return "NULL";
}

sub ProcessAlarmEnum
{
    my $shortot = shift;

    my $alarmenumname = "otai_${shortot}_alarm_type_t";

    return "&otai_metadata_enum_$alarmenumname" if defined $OTAI_ENUMS{$alarmenumname};

    return "NULL";
}


sub CreateObjectInfo
{
    WriteSectionComment "Object info metadata";

    %REVGRAPH = GetReverseDependencyGraph();

    my @objects = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        next if $1 eq "NULL" or $1 eq "MAX";

        if (not defined $OBJTOAPIMAP{$ot})
        {
            LogError "$ot is not defined in OBJTOAPIMAP, missing otai_XXX_api_t declaration?";
            next;
        }

        my $shortot = lc($1);

        my $type = "otai_" . lc($1) . "_attr_t";

        my $start = "OTAI_" . uc($1) . "_ATTR_START";
        my $end   = "OTAI_" . uc($1) . "_ATTR_END";

        my $enum  = "&otai_metadata_enum_${type}";

        my $struct = $NON_OBJECT_ID_STRUCTS{$ot};

        #
        # here we need to only generate struct member names
        # since we use those members in rev graph entries
        # so struct members must be generated previously
        #

        my $isnonobjectid       = ProcessIsNonObjectId($struct, $ot);
        my $structmembers       = ProcessStructMembersName($struct, $ot ,lc($1));
        my $structmemberscount  = ProcessStructMembersCount($struct, $ot);
        my $revgraph            = ProcessRevGraph($ot);
        my $revgraphcount       = ProcessRevGraphCount($ot);
        my $isexperimental      = ProcessIsExperimental($ot);
        my $statenum            = ProcessStatEnum($shortot);
        my $alarmenum           = ProcessAlarmEnum($shortot);
        my $attrmetalength      = @{ $OTAI_ENUMS{$type}{values} };

        my $create      = ProcessCreate($struct, $ot);
        my $remove      = ProcessRemove($struct, $ot);
        my $set         = ProcessSet($struct, $ot);
        my $get         = ProcessGet($struct, $ot);

        my $getstats    = ProcessGetStats($struct, $ot);
        my $getstatsext = ProcessGetStatsExt($struct, $ot);
        my $clearstats  = ProcessClearStats($struct, $ot);

        WriteHeader "extern const otai_object_type_info_t otai_metadata_object_type_info_$ot;";

        WriteSource "const otai_object_type_info_t otai_metadata_object_type_info_$ot = {";

        WriteSource ".objecttype           = $ot,";
        WriteSource ".objecttypename       = \"$ot\",";
        WriteSource ".attridstart          = $start,";
        WriteSource ".attridend            = $end,";
        WriteSource ".enummetadata         = $enum,";
        WriteSource ".attrmetadata         = otai_metadata_object_type_$type,";
        WriteSource ".attrmetadatalength   = $attrmetalength,";
        WriteSource ".isnonobjectid        = $isnonobjectid,";
        WriteSource ".isobjectid           = !$isnonobjectid,";
        WriteSource ".structmembers        = $structmembers,";
        WriteSource ".structmemberscount   = $structmemberscount,";
        WriteSource ".revgraphmembers      = $revgraph,";
        WriteSource ".revgraphmemberscount = $revgraphcount,";
        WriteSource ".create               = $create,";
        WriteSource ".remove               = $remove,";
        WriteSource ".set                  = $set,";
        WriteSource ".get                  = $get,";
        WriteSource ".getstats             = $getstats,";
        WriteSource ".getstatsext          = $getstatsext,";
        WriteSource ".clearstats           = $clearstats,";
        WriteSource ".isexperimental       = $isexperimental,";
        WriteSource ".statenum             = $statenum,";
        WriteSource ".alarmenum            = $alarmenum,";

        WriteSource "};";
    }

    WriteSectionComment "Object infos table";

    WriteHeader "extern const otai_object_type_info_t* const otai_metadata_all_object_type_infos[];";

    WriteSource "const otai_object_type_info_t* const otai_metadata_all_object_type_infos[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        if ($1 eq "NULL" or $1 eq "MAX")
        {
            WriteSource "NULL,";
            next;
        }

        WriteSource "&otai_metadata_object_type_info_$ot,";
    }

    WriteSource "NULL";
    WriteSource "};";
}

sub ExtractObjectsFromDesc
{
    my ($struct, $member, $desc) = @_;

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.+)/g)
    {
        my $tag = $1;
        my $val = $2;

        $val = Trim $val;

        next if not $tag eq "objects";

        return ProcessTagObjects($struct, $member, $val);
    }

    return undef;
}

sub ProcessSingleNonObjectId
{
    my $rawname = shift;

    my @types = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    my $structname = "otai_${rawname}_t";

    LogDebug "ProcessSingleNonObjectId: processing $structname";

    my $ot = "OTAI_OBJECT_TYPE_" .uc(${rawname});

    if (not grep(/$ot/,@types))
    {
        LogError "struct $structname does not correspont to known object type";
        return undef;
    }

    # NOTE: since this is a HASH then order of the members is not preserved as
    # they appear in struct definition

    my %struct = ExtractStructInfo($structname, "struct_");

    for my $member (GetStructKeysInOrder(\%struct))
    {
        my $type = $struct{$member}{type};
        my $desc = $struct{$member}{desc};

        # allowed entries on object structs

        if (not $type =~ /^otai_(nat_entry_data|mac|object_id|vlan_id|ip_address|ip_prefix|label_id|\w+_type)_t$/)
        {
            LogError "struct member $member type '$type' is not allowed on struct $structname";
            next;
        }

        next if not $type eq "otai_object_id_t";

        my $objects = ExtractObjectsFromDesc($structname, $member, $desc);

        if (not defined $objects)
        {
            LogError "no object type defined on $structname $member";
            next;
        }

        $struct{$member}{objects} = $objects;
    }

    return %struct;
}

sub ProcessNonObjectIdObjects
{
    my @rawnames = GetNonObjectIdStructNames();

    for my $rawname (@rawnames)
    {
        my %struct = ProcessSingleNonObjectId($rawname);

        my $objecttype = "OTAI_OBJECT_TYPE_" . uc($rawname);

        $NON_OBJECT_ID_STRUCTS{$objecttype} = \%struct;
    }
}

sub GetHashOfAllAttributes
{
    # list will be used to find attribute metadata
    # based on attribute string name

    my %ATTRIBUTES = ();

    for my $key (sort keys %OTAI_ENUMS)
    {
        next if not $key =~ /^(otai_(\w+)_attr_t)$/;

        my $typedef = $1;

        my $enum = $OTAI_ENUMS{$typedef};

        my @values = @{ $enum->{values} };

        for my $attr (@values)
        {
            if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$attr})
            {
                LogError "metadata is missing for $attr";
                next;
            }

            next if defined $METADATA{$typedef}{$attr}{ignore};

            $ATTRIBUTES{$attr} = 1;
        }
    }

    return %ATTRIBUTES;
}

sub CheckCapabilities
{
    my %ATTRIBUTES = GetHashOfAllAttributes();

    for my $attr (keys %CAPABILITIES)
    {
        next if defined $ATTRIBUTES{$attr};

        LogError "capability attribute $attr not found on all attributes list";
    }
}

sub CreateListOfAllAttributes
{
    # list will be used to find attribute metadata
    # based on attribute string name

    WriteSectionComment "List of all attributes";

    my %ATTRIBUTES = GetHashOfAllAttributes();

    WriteHeader "extern const otai_attr_metadata_t* const otai_metadata_attr_sorted_by_id_name[];";
    WriteSource "const otai_attr_metadata_t* const otai_metadata_attr_sorted_by_id_name[] = {";

    my @keys = sort keys %ATTRIBUTES;

    for my $attr (@keys)
    {
        WriteSource "&otai_metadata_attr_$attr,";
    }

    my $count = @keys;

    WriteSource "NULL";
    WriteSource "};";

    WriteSource "const size_t otai_metadata_attr_sorted_by_id_name_count = $count;";
    WriteHeader "extern const size_t otai_metadata_attr_sorted_by_id_name_count;";
}

sub GetHashOfAllStatistics
{
    my %STATISTICS = ();

    for my $key (sort keys %OTAI_ENUMS)
    {
        next if not $key =~ /^(otai_(\w+)_stat_t)$/;

        my $typedef = $1;

        my $enum = $OTAI_ENUMS{$typedef};

        my @values = @{ $enum->{values} };

        for my $stat (@values)
        {
            if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$stat})
            {
                LogError "metadata is missing for $stat";
                next;
            }

            $STATISTICS{$stat} = 1;
        }
    }

    return %STATISTICS;
}

sub CreateListOfAllStatistics
{
    WriteSectionComment "List of all statistics";

    my %STATISTICS = GetHashOfAllStatistics();

    WriteHeader "extern const otai_stat_metadata_t* const otai_metadata_stat_sorted_by_id_name[];";
    WriteSource "const otai_stat_metadata_t* const otai_metadata_stat_sorted_by_id_name[] = {";

    my @keys = sort keys %STATISTICS;

    for my $stat (@keys)
    {
        WriteSource "&otai_metadata_stat_$stat,"
    }

    my $count = @keys;

    WriteSource "NULL";
    WriteSource "};";

    WriteSource "const size_t otai_metadata_stat_sorted_by_id_name_count = $count;";
    WriteHeader "extern const size_t otai_metadata_stat_sorted_by_id_name_count;";
}

sub CheckApiStructNames
{
    #
    # purpose of this check is to find out
    # whether otai_api_t enums match actual
    # struct of api declarations
    #

    my @values = @{ $OTAI_ENUMS{"otai_api_t"}{values} };

    for my $value (@values)
    {
        next if $value eq "OTAI_API_UNSPECIFIED";

        if (not $value =~ /^OTAI_API_(\w+)$/)
        {
            LogError "invalie api name $value";
            next;
        }

        my $api = lc $1;

        my $structName = "otai_${api}_api_t";

        my $structFile = "struct_$structName.xml";

        # doxygen doubles underscores

        $structFile =~ s/_/__/g;

        my $file = "$XMLDIR/$structFile";

        if (not -e $file)
        {
            LogError "there is no struct $structName corresponding to api name $value";
        }
    }

    for my $name (sort keys %ALL_STRUCTS)
    {
        next if not $name =~ /^otai_(\w+)_api_t$/;

        my $val = uc("OTAI_API_$1");

        if (not grep(/^$val$/,@values))
        {
            LogError "struct '$name' defined, but enum entry $val is missing on otai_api_t";
        }
    }
}

sub CheckApiDefines
{
    #
    # purpose of this check is to check whether
    # all enum entries defined in otai_api_t
    # have corresponding structs defined for each
    # defined object like otai_fdb_api_t
    #

    my @apis = @{ $OTAI_ENUMS{otai_api_t}{values} };

    for my $api (@apis)
    {
        my $short = lc($1) if $api =~ /OTAI_API_(\w+)/;

        next if $short eq "unspecified";

        if (not defined $APITOOBJMAP{$short})
        {
            LogError "$api is defined in otai.h but no corresponding struct for objects found";
        }
    }
}

sub ExtractStatsFunctionMap
{
    #
    # Purpose is to get statistics functions consistent
    # with stat_t defined for them
    #

    my @headers = GetHeaderFiles();

    my @merged = @headers;

    my %otmap = ();

    for my $header (@merged)
    {
        my $data = ReadHeaderFile($header);

        next if not $data =~ m!(otai_\w+_api_t)(.+?)\1;!igs;

        my $apis = $2;

        my @fns = $apis =~ /otai_(\w+_stats(?:_ext)?)_fn/g;

        for my $fn (@fns)
        {
            # exceptions

            next if $fn eq "clear_port_all_stats";
            next if $fn eq "get_tam_snapshot_stats";

            if (not $fn =~ /^(?:get|clear)_(\w+)_stats(?:_ext)?$/)
            {
                LogWarning "Invalid stats function name: $fn";
            }

            my $ot = $1;
            my @statfns = ();

            $otmap{$ot} = \@statfns if not defined $otmap{$ot};

            my $ref = $otmap{$ot};

            push@$ref,$fn;
        }
    }

    %OBJECT_TYPE_TO_STATS_MAP = %otmap;
}

sub CheckObjectTypeStatitics
{
    #
    # Purpose is to check if each defined statistics for object type has 3 stat
    # functions defined and if there is corresponding object type for stat enum
    #

    for my $ot (sort keys %OBJECT_TYPE_TO_STATS_MAP)
    {
        my $ref = $OBJECT_TYPE_TO_STATS_MAP{$ot};
        my $stats = "@$ref";

        # each object type that supports statistics should have 3 stat functions (and in that order)

        my $expected = "get_${ot}_stats get_${ot}_stats_ext clear_${ot}_stats";

        next if $stats eq $expected;

        LogWarning uc($ot) . " has only '$stats' functions, expected: $expected";
    }

    for my $key (keys %OTAI_ENUMS)
    {
        next if not $key =~ /otai_(\w+)_stat_t/;

        my $ot = $1;

        next if defined $OBJECT_TYPE_TO_STATS_MAP{$ot};

        LogWarning "stats $key are defined, but no API 3 stat functions defined for $ot";
    }
}

sub ExtractApiToObjectMap
{
    #
    # Purpose is to get which object type
    # maps to which API, since multiple object types like acl
    # can map to one api structure
    #

    my @headers = GetHeaderFiles();

    my @merged = @headers;

    for my $header (@merged)
    {
        my $data = ReadHeaderFile($header);

        my @lines = split/\n/,$data;

        my $empty = 0;
        my $emptydoxy = 0;

        my @objects = ();
        my $api = undef;

        for my $line (@lines)
        {
            if ($line =~ /typedef\s+enum\s+_otai_(\w+)_attr_t/)
            {
                push@objects,uc("OTAI_OBJECT_TYPE_$1");
            }

            if ($line =~ /typedef\s+struct\s+_otai_(\w+)_api_t/)
            {
                $api = $1;
                last;
            }
        }

        if (not defined $api)
        {
            my $len = @objects;

            if ($len > 0)
            {
                LogError "api struct was not found in file $header, but objects are defined @objects";
                next;
            }

            next;
        }

        my $shortapi = $api;

        $shortapi =~ s/_//g;

        my $correct = "otai$shortapi.h";

        if ($header ne $correct)
        {
            LogWarning "File $header should be named '$correct'";
        }

        # NOTE: those maps will include experimental extensions

        for my $obj(@objects)
        {
            $OBJTOAPIMAP{$obj} = $api;

            $EXPERIMENTAL_OBJECTS{uc($obj)} = 1 if $correct =~ /^otaiexperimental/;
        }

        $APITOOBJMAP{$api} = \@objects;
    }
}

sub GetReverseDependencyGraph
{
    #
    # Purpose of this method is to generate reverse
    # dependency graph of where object ID are used
    #

    my %REVGRAPH = ();

    my @objects = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^OTAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $otname = $1;

        my $typedef = lc "otai_${otname}_attr_t";

        next if $ot =~ /^OTAI_OBJECT_TYPE_(MAX|NULL)$/;

        # for each objec types we need to scann all objects
        # also non object id structs

        my $enum = $OTAI_ENUMS{$typedef};

        my @values = @{ $enum->{values} };

        for my $attr (@values)
        {
            # metadata of single attribute of this object type

            my $meta = $METADATA{$typedef}{$attr};

            next if not defined $meta->{objects};

            # we will also include RO attributes

            my @objects = @{ $meta->{objects} };

            my $attrid = $meta->{attrid};

            for my $usedot (@objects)
            {
                if (not defined $REVGRAPH{$usedot})
                {
                    my @arr = ();

                    $REVGRAPH{$usedot} = \@arr;
                }

                my $ref = $REVGRAPH{$usedot};

                push@$ref,"$ot,$attrid";
            }
        }

        next if not defined $NON_OBJECT_ID_STRUCTS{$ot};

        # handle non object id types

        my %struct = %{ $NON_OBJECT_ID_STRUCTS{$ot} };

        for my $key (sort keys %struct)
        {
            next if not defined $struct{$key}{objects};

            my @objs = @{ $struct{$key}{objects} };

            for my $usedot (@objs)
            {
                if (not defined $REVGRAPH{$usedot})
                {
                    my @arr = ();

                    $REVGRAPH{$usedot} = \@arr;
                }

                my $ref = $REVGRAPH{$usedot};

                push@$ref,"$ot,$key";
            }
        }
    }

    return %REVGRAPH;
}

sub WriteLoggerVariables
{
    #
    # logger requires 2 variables
    # - log level
    # - log function
    #
    # we can extract this to another source file otaimetadatalogger.c
    # but now seems to be unnecessary
    #

    WriteSectionComment "Loglevel variables";

    WriteSource "volatile otai_log_level_t otai_metadata_log_level = OTAI_LOG_LEVEL_NOTICE;";
    WriteSource "volatile otai_metadata_log_fn otai_metadata_log = NULL;";
}

my %ProcessedItems = ();

sub ProcessStructItem
{
    my ($type, $struct) = @_;

    $type = $1 if $struct =~ /^otai_(\w+)_list_t$/ and $type =~ /^(\w+)\*$/;

    return if defined $ProcessedItems{$type};

    return if defined $OTAI_ENUMS{$type}; # struct entry is enum

    return if $type eq "bool";

    return if $type =~ /^otai_(u?int\d+|ip[46]|mac|cos|vlan_id|queue_index)_t/; # primitives, we could get that from defines
    return if $type =~ /^u?int\d+_t/;
    return if $type =~ /^otai_[su]\d+_list_t/;
    return if $type =~ /^otai_double_t/;

    if ($type eq "otai_object_id_t" or $type eq "otai_object_list_t")
    {
        # NOTE: don't change that, we can't have object id's inside complicated structures

        LogError "type $type in $struct can't be used, please convert struct to new object type and this item to an attribute";
        return;
    }

    my %S = ();

    if ($type =~ /^union (\w+)::(\w+)\s*$/)
    {
        # union is special, but now since all unions are named
        # then members are not flattened anyway, and we need to examine
        # entries from union xml
        # XXX may require revisit if union names will be complicated

        my $unionStructName = $1;
        my $unionName = $2;

        $unionStructName =~ s/_/__/g;
        $unionName =~ s/_/__/g;

        my $filename = "union${unionStructName}_1_1$unionName.xml";

        %S = ExtractStructInfo($unionStructName, $filename);
    }
    else
    {
        %S = ExtractStructInfo($type, "struct_");
    }

    for my $key (sort keys %S)
    {
        my $item = $S{$key}{type};

        ProcessStructItem($item, $type);

        $ProcessedItems{$item} = 1;
    }
}

sub CheckAttributeValueUnion
{
    #
    # purpose of this test is to find out if attribute
    # union contains complex structures members that also contain
    # object id, all object ids should be simple object id member oid
    # or object list objlist, other complext structures containing
    # objects are NOT supported since it will be VERY HARD to track
    # object dependencies via metadata and comparison logic
    #

    my %Union = ExtractStructInfo("otai_attribute_value_t", "union_");

    my @primitives = qw/otai_pointer_t otai_object_id_t otai_object_list_t char/;

    for my $key (sort keys %Union)
    {
        my $type = $Union{$key}{type};

        next if $type eq "char[512]";
        next if $type =~ /otai_u?int\d+_t/;
        next if $type =~ /otai_float_t/;
        next if $type =~ /otai_[su]\d+_list_t/;

        next if defined $PRIMITIVE_TYPES{$type};

        next if grep(/^$type$/, @primitives);

        ProcessStructItem($type, "otai_attribute_value_t");
    }
}

sub CheckStatEnum
{
    for my $key (keys %OTAI_ENUMS)
    {
        next if not $key =~ /otai_(\w+)_stat_t/;

        my $ot = uc("OTAI_OBJECT_TYPE_$1");

        next if defined $OBJECT_TYPE_MAP{$ot};

        LogError "stat enum defined $key but no object type $ot exists";
    }
}

sub CreateNotificationStruct
{
    #
    # create notification struct for easier notification
    # manipulation in code
    #

    WriteSectionComment "OTAI notifications struct";

    WriteHeader "typedef struct _otai_linecard_notifications_t {";

    for my $name (sort keys %NOTIFICATIONS)
    {
        if (not $name =~ /^otai_(\w+)_notification_fn/)
        {
            LogWarning "notification function $name is not ending on _notification_fn";
            next;
        }

        WriteHeader "$name on_$1;";
    }

    WriteHeader "} otai_linecard_notifications_t;";
}

sub CreateNotificationEnum
{
    #
    # create notification enum for easier notification
    # manipulation in code
    #

    WriteSectionComment "OTAI notifications enum";

    my $typename = "otai_linecard_notification_type_t";

    WriteHeader "typedef enum _$typename {";

    my $prefix = uc $typename;

    chop $prefix;

    my @values = ();

    for my $name (sort keys %NOTIFICATIONS)
    {
        if (not $name =~ /^otai_(\w+)_notification_fn/)
        {
            LogWarning "notification function '$name' is not ending on _notification_fn";
            next;
        }

        $name = uc $1;

        WriteHeader "${prefix}$name,";

        push @values, "${prefix}$name";
    }

    WriteHeader "} $typename;";

    $OTAI_ENUMS{$typename}{values} = \@values;

    WriteSectionComment "otai_linecard_notification_type_t metadata";

    ProcessSingleEnum($typename, $typename, $prefix);

    WriteSectionComment "Get otai_linecard_notification_type_t helper method";

    CreateEnumHelperMethod("otai_linecard_notification_type_t");
}

sub CreateSwitchNotificationAttributesList
{
    #
    # create notification attributes list for easy use on places where only
    # notifications must be processed instead of looping through all linecard
    # attributes
    #

    WriteSectionComment "OTAI Switch Notification Attributes List";

    WriteHeader "extern const otai_attr_metadata_t* const otai_metadata_linecard_notify_attr[];";
    WriteSource "const otai_attr_metadata_t* const otai_metadata_linecard_notify_attr[] = {";

    for my $name (sort keys %NOTIFICATIONS)
    {
        next if not $name =~ /^otai_(\w+)_notification_fn/;

        WriteSource "&otai_metadata_attr_OTAI_LINECARD_ATTR_" . uc($1) . "_NOTIFY,";
    }

    WriteSource "NULL";
    WriteSource "};";

    my $count = scalar(keys %NOTIFICATIONS);

    WriteHeader "extern const size_t otai_metadata_linecard_notify_attr_count;";
    WriteSource "const size_t otai_metadata_linecard_notify_attr_count = $count;";

    WriteSectionComment "Define OTAI_METADATA_LINECARD_NOTIFY_ATTR_COUNT";

    WriteHeader "#define OTAI_METADATA_LINECARD_NOTIFY_ATTR_COUNT $count";
}

sub WriteHeaderHeader
{
    WriteSectionComment "AUTOGENERATED FILE! DO NOT EDIT";

    WriteHeader "#ifndef __OTAI_METADATA_H__";
    WriteHeader "#define __OTAI_METADATA_H__";

    WriteHeader "#include <otai.h>";

    WriteHeader "#include \"otaimetadatatypes.h\"";
    WriteHeader "#include \"otaimetadatautils.h\"";
    WriteHeader "#include \"otaimetadatalogger.h\"";
    WriteHeader "#include \"otaiserialize.h\"";
}

sub WriteHeaderFotter
{
    WriteHeader "#endif /* __OTAI_METADATA_H__ */";
}

sub ProcessXmlFiles
{
    for my $file (GetLaiXmlFiles($XMLDIR))
    {
        LogInfo "Processing $file";

        ProcessXmlFile("$XMLDIR/$file");
    }
}

sub ProcessValues
{
    my ($refUnion, $refValueTypes, $refValueTypesToVt) = @_;

    for my $key (keys %$refUnion)
    {
        my $type = $refUnion->{$key}->{type};

        next if $type eq "char[512]" or $type eq "bool";

        if (not $type =~ /^otai_(\w+)_t$/)
        {
            LogWarning "skipping type $type, FIXME";
            next;
        }

        my $innername = $1;

        $innername =~ s/^s(\d+)/INT$1/;
        $innername =~ s/^u(\d+)/UINT$1/;
        $innername =~ s/^ip(\d+)/IPV$1/;

        $refValueTypes->{$type} = $key;
        $refValueTypesToVt->{$type} = uc($innername);
    }
}

sub PopulateValueTypes
{
    my %Union = ExtractStructInfo("otai_attribute_value_t", "union_");

    ProcessValues(\%Union, \%VALUE_TYPES, \%VALUE_TYPES_TO_VT);
}

sub CreateObjectTypeMap
{
    map { $OBJECT_TYPE_MAP{$_} = $_ } @{ $OTAI_ENUMS{otai_object_type_t}{values} };
}

sub ExtractUnionsInfo
{
    my @files = GetXmlUnionFiles($XMLDIR);

    for my $file (@files)
    {
        my $ref = ReadXml $file;

        my $kind = $ref->{compounddef}[0]->{kind};

        if ($kind ne "union")
        {
            LogError "expected '$file' to contain union but kind is '$kind'";
            next;
        }

        my $def = $ref->{compounddef}[0]->{compoundname}[0];

        if (not $def =~ /^(_otai_\w+::)*_(\w+)$/)
        {
            LogWarning "union name '$def' not match pattern: (_otai_\\w+::)*(_\\w+)";
            next;
        }

        my $name = $2;

        LogError "Name $name should be in format otai_\\w+_t" if not $name =~ /^otai_\w+_t$/;

        $OTAI_UNIONS{$name}{file}    = $file;
        $OTAI_UNIONS{$name}{name}    = $name;
        $OTAI_UNIONS{$name}{def}     = $def;
        $OTAI_UNIONS{$name}{nested}  = 1 if $def =~ /::/;

        my %s = ExtractStructInfoEx($name, $file);

        # NOTE: validonly tag must exists on each member and is checked on serialize function
        # NOTE: extraparam tag must on struct description and is checked on serialize function
    }
}

sub LoadCapabilities
{
    %CAPABILITIES = %{ GetCapabilities() };
}

sub MergeExtensionsEnums
{
    for my $exenum (sort keys%EXTENSIONS_ENUMS)
    {
        if (not $exenum =~ /^(otai_\w+)_extensions_t$/)
        {
            LogError "Enum $exenum is not extension enum";
            next;
        }

        my $enum = "$1_t";

        if (not defined $OTAI_ENUMS{$enum})
        {
            LogError "Enum $exenum is extending not existing enum $enum";
            next;
        }

        my @exvalues = @{ $OTAI_ENUMS{$exenum}{values} };

        my @values = @{ $OTAI_ENUMS{$enum}{values} };

        push@values,@exvalues;

        $OTAI_ENUMS{$enum}{values} = \@values;

        next if not $exenum =~ /_attr_extensions_t/;

        for my $exvalue (@exvalues)
        {
            $EXTENSIONS_ATTRS{$exvalue} = 1;

            $METADATA{$enum}{$exvalue} = $METADATA{$exenum}{$exvalue};
        }
    }
}

sub ProcessNotificationStruct
{
    my $rawname = shift;

    my @types = @{ $OTAI_ENUMS{otai_object_type_t}{values} };

    my $structname = "otai_${rawname}_t";

    LogDebug "ProcessProcessNotificationStruct: processing $structname";

    my %struct = ExtractStructInfo($structname, "struct_");

    #print Dumper(%OTAI_ENUMS);
    for my $member (GetStructKeysInOrder(\%struct))
    {
        my $type = $struct{$member}{type};
        my $desc = $struct{$member}{desc};

        # allowed entries on notification object structs

        next if defined $OTAI_ENUMS{$type};          # type is enum !
        next if $type =~ /^otai_\w+_entry_t/;        # non object id struct
        next if $type =~ /^(uint32_t|bool)$/;

        if ($type =~ /^(otai_object_id_t|otai_attribute_t\*)$/)
        {
            my $objects = ExtractObjectsFromDesc($structname, $member, $desc);

            if (not defined $objects)
            {
                LogError "no object type defined on $structname $member";
                next;
            }

            $struct{$member}{objects} = $objects;
            next;
        }

        LogWarning "$member $type";
    }

    return %struct;
}

sub CreateOtherStructs
{
    WriteSectionComment "Notifications structs members metadata";

    my @ntfstructs = ();

    for my $name (sort keys %ALL_STRUCTS)
    {
        next if $name =~ /^otai_\w+_(api|list|entry)_t$/;

        next if not $name =~ /^otai_(\w+_notification(_data)?)_t$/;

        my $rawname = $1;

        my %struct = ProcessNotificationStruct($rawname);

        my$membersname = ProcessStructMembers(\%struct, "NULL", $rawname, 1);

        push@ntfstructs, $membersname;
    }

    for my $name(@ntfstructs)
    {
        WriteHeader "extern const otai_struct_member_info_t* const $name\[\];";
    }
}

#
# MAIN
#

LoadCapabilities();

ExtractApiToObjectMap();

ExtractStatsFunctionMap();

ExtractUnionsInfo();

CheckHeadersStyle() if not defined $optionDisableStyleCheck;

GetStructLists();

PopulateValueTypes();

ProcessXmlFiles();

MergeExtensionsEnums();

CreateObjectTypeMap();

WriteHeaderHeader();

ProcessLaiStatus();

ProcessExtraRangeDefines();

CreateMetadataHeaderAndSource();

CreateMetadata();

CreateMetadataForAttributes();

CreateMetadataForStatistics();

CreateEnumHelperMethods();

ProcessNonObjectIdObjects();

CreateOtherStructs();

CreateStructNonObjectId();

CreateApis();

CreateApisStruct();

CreateGlobalApis();

CreateApisQuery();

CreateObjectInfo();

CreateListOfAllAttributes();

CreateListOfAllStatistics();

CheckCapabilities();

CheckApiStructNames();

CheckApiDefines();

CheckAttributeValueUnion();

CheckStatEnum();

#CheckObjectTypeStatitics();

CreateNotificationStruct();

CreateNotificationEnum();

CreateSwitchNotificationAttributesList();

CreateSerializeMethods();

WriteHeaderFotter();

WriteLoggerVariables();

WriteMetaDataFiles();

GenLaiSpecFile();
