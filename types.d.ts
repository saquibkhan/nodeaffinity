/**
 * Sets the affinity of the main Node JavaScript thread
 * @returns The bit mask, -1 on failure
 */
export declare function getAffinity(): number;

/**
 * Sets the affinity of the main Node JavaScript thread
 * @returns The new mask, -1 on failure
 * @param mask {Number} bit mask of which cores to run on
 */
export declare function setAffinity(mask: number): number;