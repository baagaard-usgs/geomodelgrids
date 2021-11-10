"""Rules defining elastic properties given x, y, depth for version
08.3.0 of the USGS San Francisco Bay 3-D seismic velocity model (regional domain).

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
        "mantle": rules_aagaard_etal_2010.upper_mantle,
        "mafic lower crust": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "GV gabbro": rules_aagaard_etal_2010.mafic_great_valley_ophiolite,
        "KJf_Foothills": rules_aagaard_etal_2010.franciscan_foothills,
        "KJf_W_Diablo": rules_aagaard_etal_2010.franciscan_berkeley,
        "KJf_E_Diablo": rules_aagaard_etal_2010.franciscan_berkeley,
        "KJf_Sur": rules_aagaard_etal_2010.franciscan_berkeley,
        "KJf_Viscaino": rules_aagaard_etal_2010.franciscan_foothills,
        "KJf_Coastal_Belt": rules_aagaard_etal_2010.franciscan_berkeley,
        "<null>": rules_aagaard_etal_2010.outside_model,
        "K": rules_aagaard_etal_2010.great_valley_sequence_sedimentary,
        "Kgr": rules_aagaard_etal_2010.salinian_granitic,
        "KJF_Bay_Block": rules_aagaard_etal_2010.franciscan_napa_sonoma,
        "Cs_Coast Ranges": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Cs_Offshore": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Foothills": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Great_Valley": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Salinia": rules_aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Q_Salinia": rules_aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "water": rules_aagaard_etal_2010.seawater,
        "": rules_aagaard_etal_2010.outside_model,
    }
    return RULES[zone]
