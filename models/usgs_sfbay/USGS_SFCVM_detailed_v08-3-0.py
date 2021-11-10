"""Rules defining elastic properties given x, y, depth for version
08.3.0 of the USGS San Francisco Bay 3-D seismic velocity model.

Vp and Vs are in km/s. Density is in g/cm**3.

"""

import rules_aagaard_etal_2010


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
        "Mantle": rules_aagaard_etal_2010.upper_mantle,
        "Lower Crust": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Great_Valley_Ophiolite": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "San Leandro G": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Logan G": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Kjf_Berkeley": rules_aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Chabot": rules_aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Foothills": rules_aagaard_etal_2010.franciscan_foothills,
        "Kjf_Merced": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Sur": rules_aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Napa_Somoma": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Bay Block": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Evergreen": rules_aagaard_etal_2010.franciscan_berkeley,
        "Kjf_E_Diablo": rules_aagaard_etal_2010.franciscan_berkeley,
        "Kjf_W_Diablo": rules_aagaard_etal_2010.franciscan_berkeley,
        "Valley Sequence": rules_aagaard_etal_2010.great_valley_sequence_sedimentary,
        "Kgr_Gab": rules_aagaard_etal_2010.salinian_granitic,
        "Kgr_Halfmoon": rules_aagaard_etal_2010.salinian_granitic,
        "Kgr_SCoast": rules_aagaard_etal_2010.salinian_granitic,
        "Kgr_NShelf": rules_aagaard_etal_2010.salinian_granitic,
        "Great Valley Seq": rules_aagaard_etal_2010.great_valley_sequence_sedimentary,
        "Cenozoic_Great Valley": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Cenozoic_Halfmoon": rules_aagaard_etal_2010.cenozoic_sedimentary_halfmoonbay,
        "Cenozoic_Ever": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_SouthBay": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_La Honda": rules_aagaard_etal_2010.tertiary_sedimentary_lahondabasin,
        "T_Pilarcitos": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Bay_Block_Santa_Rosa": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Berkeley": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Cenozoic": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "QT_Bay_Block_Santa Rosa": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "QT_Merced_Pilarcitos": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "QT_South_Bay": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "<null>": rules_aagaard_etal_2010.outside_model,
        "water": rules_aagaard_etal_2010.seawater,
        "": rules_aagaard_etal_2010.outside_model,
    }
    return RULES[zone]
