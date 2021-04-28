"""Rules defining elastic properties given x, y, depth for version
08.3.0 of the USGS San Francisco Bay 3-D seismic velocity model.

Vp and Vs are in km/s. Density is in g/cm**3.

"""

import rules_aagaard_etal_2010
import rules_hirakawa_aagaard_2021


def get_rules(fault_block, zone):
    """Get rules for fault block and zone names.

    In this model the rules depend only on the zone; they do NOT
    vary from fault block to fault block for a given zone.

    Args:
        fault_block (str)
            Name of fault block.
        zone (str)
            Zone name.

    Returns:
        Function (rule) that computes elastic properties (density, Vp, Vs, Qp, and Qs) from x, y, depth.

    """
    RULES = {
        "Mantle": {
            "default": rules_aagaard_etal_2010.upper_mantle,
        },
        "Lower Crust": {
            "default": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        },
        "Great_Valley_Ophiolite": {
            "default": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        },
        "San Leandro G": {
            "default": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        },
        "Logan G": {
            "default": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        },
        "Kjf_Berkeley": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kjf_Chabot": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kjf_Foothills": {
            "default": rules_aagaard_etal_2010.franciscan_foothills,
        },
        "Kjf_Merced": {
            "default": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        },
        "Kjf_Sur": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kjf_Napa_Somoma": {
            "default": rules_aagaard_etal_2010.franciscan_napa_sonoma,
            "Napa": rules_hirakawa_aagaard_2021.franciscan_napa,
            "Sonoma": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        },
        "Kjf_Bay Block": {
            "default": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        },
        "Kjf_Evergreen": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kjf_E_Diablo": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kjf_W_Diablo": {
            "default": rules_aagaard_etal_2010.franciscan_berkeley,
        },
        "Kgr_Gab": {
            "default": rules_aagaard_etal_2010.salinian_granitic,
        },
        "Kgr_Halfmoon": {
            "default": rules_aagaard_etal_2010.salinian_granitic,
        },
        "Kgr_SCoast": {
            "default": rules_aagaard_etal_2010.salinian_granitic,
        },
        "Kgr_NShelf": {
            "default": rules_aagaard_etal_2010.salinian_granitic,
        },
        "Valley Sequence": {
            "default": rules_aagaard_etal_2010.great_valley_sequence_sedimentary,
            "San Leandro": rules_hirakawa_aagaard_2021.valley_sequence_sanleandro,
            "Sunol": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
        },
        "Great Valley Seq": {
            "default": rules_aagaard_etal_2010.great_valley_sequence_sedimentary,
            "Berkeley": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
            "Chabot": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
            "W Diablo Range": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
        },
        "Cenozoic": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
            "Napa": rules_hirakawa_aagaard_2021.cenozoic_napa,
            "Sonoma": rules_hirakawa_aagaard_2021.cenozoic_sonoma,
            "Alexander": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
            "W Diablo Range": rules_hirakawa_aagaard_2021.quaternary_livermore,
        },
        "Cenozoic_Great Valley": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        },
        "Cenozoic_Halfmoon": {
            "default": rules_aagaard_etal_2010.cenozoic_sedimentary_halfmoonbay,
        },
        "Cenozoic_Ever": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        },
        "T_SouthBay": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        },
        "T_La Honda": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_lahondabasin,
        },
        "T_Pilarcitos": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        },
        "T_Bay_Block_Santa_Rosa": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
            "Santa Rosa": rules_hirakawa_aagaard_2021.brocher2005_older_cenozoic_sedimentary,
        },
        "T_Berkeley": {
            "default": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
            "Berkeley": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
            "Chabot": rules_hirakawa_aagaard_2021.brocher2008_great_valley_sequence,
        },
        "QT_Bay_Block_Santa Rosa": {
            "default": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        },
        "QT_Merced_Pilarcitos": {
            "default": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        },
        "QT_South_Bay": {
            "default": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        },
        "water": {
            "default": rules_aagaard_etal_2010.seawater,
        },
        "<null>": {
            "default": rules_aagaard_etal_2010.outside_model,
        },
        "": {
            "default": rules_aagaard_etal_2010.outside_model,
        },
    }

    return RULES[zone].get(fault_block, RULES[zone]["default"])
