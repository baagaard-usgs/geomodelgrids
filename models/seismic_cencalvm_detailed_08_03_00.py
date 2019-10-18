"""Rules defining elastic properties given x, y, depth for version
08.3.0 of the USGS San Francisco Bay 3-D seismic velocity model.

Vp and Vs are in km/s. Density is in g/cm**3.

"""

import aagaard_etal_2010


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
        "Mantle": aagaard_etal_2010.upper_mantle,
        "Lower Crust": aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Great_Valley_Ophiolite": aagaard_etal_2010.mafic_great_valley_ophiolite,
        "San Leandro G": aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Logan G": aagaard_etal_2010.mafic_great_valley_ophiolite,
        "Kjf_Berkeley": aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Chabot": aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Foothills": aagaard_etal_2010.franciscan_foothills,
        "Kjf_Merced": aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Sur": aagaard_etal_2010.franciscan_berkeley,
        "Kjf_Napa_Somoma": aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Bay Block": aagaard_etal_2010.franciscan_napa_sonoma,
        "Kjf_Evergreen": aagaard_etal_2010.franciscan_berkeley,
        "Kjf_E_Diablo": aagaard_etal_2010.franciscan_berkeley,
        "Kjf_W_Diablo": aagaard_etal_2010.franciscan_berkeley,
        "Valley Sequence": aagaard_etal_2010.great_valley_sequence_sedimentary,
        "Kgr_Gab": aagaard_etal_2010.salinian_granitic,
        "Kgr_Halfmoon": aagaard_etal_2010.salinian_granitic,
        "Kgr_SCoast": aagaard_etal_2010.salinian_granitic,
        "Kgr_NShelf": aagaard_etal_2010.salinian_granitic,
        "Great Valley Seq": aagaard_etal_2010.great_valley_sequence_sedimentary,
        "Cenozoic_Great Valley": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Cenozoic_Halfmoon": aagaard_etal_2010.cenozoic_sedimentary_halfmoonbay,
        "Cenozoic_Ever": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_SouthBay": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_La Honda": aagaard_etal_2010.tertiary_sedimentary_lahondabasin,
        "T_Pilarcitos": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Bay_Block_Santa_Rosa": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "T_Berkeley": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "Cenozoic": aagaard_etal_2010.tertiary_sedimentary_southbay,
        "QT_Bay_Block_Santa Rosa": aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "QT_Merced_Pilarcitos": aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "QT_South_Bay": aagaard_etal_2010.quaternary_tertiary_sedimentary,
        "<null>": aagaard_etal_2010.outside_model,
        "water": aagaard_etal_2010.seawater,
        "": aagaard_etal_2010.outside_model,
    }
    return RULES[zone]
