/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "e2sm_kpm_v02.03_modified.asn1"
 * 	`asn1c -gen-APER -no-gen-OER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps`
 */

#include "InterfaceType.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
static asn_per_constraints_t asn_PER_type_InterfaceType_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  3,  3,  0,  6 }	/* (0..6,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_InterfaceType_value2enum_1[] = {
	{ 0,	2,	"nG" },
	{ 1,	2,	"xn" },
	{ 2,	2,	"f1" },
	{ 3,	2,	"e1" },
	{ 4,	2,	"s1" },
	{ 5,	2,	"x2" },
	{ 6,	2,	"w1" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_InterfaceType_enum2value_1[] = {
	3,	/* e1(3) */
	2,	/* f1(2) */
	0,	/* nG(0) */
	4,	/* s1(4) */
	6,	/* w1(6) */
	5,	/* x2(5) */
	1	/* xn(1) */
	/* This list is extensible */
};
static const asn_INTEGER_specifics_t asn_SPC_InterfaceType_specs_1 = {
	asn_MAP_InterfaceType_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_InterfaceType_enum2value_1,	/* N => "tag"; sorted by N */
	7,	/* Number of elements in the maps */
	8,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_InterfaceType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_InterfaceType = {
	"InterfaceType",
	"InterfaceType",
	&asn_OP_NativeEnumerated,
	asn_DEF_InterfaceType_tags_1,
	sizeof(asn_DEF_InterfaceType_tags_1)
		/sizeof(asn_DEF_InterfaceType_tags_1[0]), /* 1 */
	asn_DEF_InterfaceType_tags_1,	/* Same as above */
	sizeof(asn_DEF_InterfaceType_tags_1)
		/sizeof(asn_DEF_InterfaceType_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		&asn_PER_type_InterfaceType_constr_1,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_InterfaceType_specs_1	/* Additional specs */
};

